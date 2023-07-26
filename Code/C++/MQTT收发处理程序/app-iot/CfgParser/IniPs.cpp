#include "IniPs.h"

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define     PRI(fmt, ...)  printf("[%s:%02d] "       fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)
#define PRI_INF(fmt, ...)  printf("[%s:%02d][INF]: " fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)
#define PRI_ERR(fmt, ...)  printf("[%s:%02d][ERR]: " fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)

using namespace LLZ;

IniPs::IniPs(const char *filePath)
{
    this->open(filePath, 0);
}

IniPs::IniPs(const char *IniStr, int strLen)
{
    this->open(IniStr, strLen);
}

IniPs::~IniPs()
{
}

int IniPs::cfg_flush()
{
    this->m->ini.Reset();
    switch ( m->workMode )
    {
        case IniPs_s::FILEMODE:      // ini文件模式
        {
            this->m->rc = this->m->ini.LoadFile(m->filePath.c_str());
            break;
        }

        case IniPs_s::STRMODE:       // ini字符串模式
            this->m->rc = this->m->ini.LoadData(m->IniStr);
            break;
        
        default:
            PRI_ERR("cfg_flush(): workMode is default");
            return -1;
    }
    
    if ( this->m->rc <= SI_FAIL )
    {
        PRI_ERR("read %s error(%d)", (m->workMode == IniPs_s::FILEMODE) ? m->filePath.c_str() : m->IniStr.c_str(), this->m->rc);
        return -1;
    }
    this->m->initFlg = true;
    
    return 0;
}

bool IniPs::get_init()
{
    return m->initFlg;
}

int IniPs::open(const char *filePathOrStr, int steLen)
{
    if ( steLen <= 0 )
    {
        m->filePath = filePathOrStr;
        m->workMode = IniPs_s::FILEMODE;
    }
    else
    {
        m->IniStr = filePathOrStr;
        m->workMode = IniPs_s::STRMODE;
    }

    cfg_flg_init();
    cfg_flush();
    
    return 0;
}

inline void IniPs::cfg_flg_init()
{
    this->m->ini.SetUnicode();
}

int IniPs::pick_keyPath(const char *keyPath, vector<string>& oVecStr)
{
    int ret = 0;
    std::string token;
    if ( *keyPath == '/' )
    {
        keyPath+=1;
    }
    std::istringstream iss(keyPath);
    oVecStr.clear();

    // 使用getline函数将字符串分割成子字符串
    while (std::getline(iss, token, '/')) 
    {
        ret++;
        oVecStr.push_back(token);
    }
    return ret;
}

void* IniPs::cfg_get_val(const char *keyPath)
{
    if ( get_init() == false )
    {
        return nullptr;
    }
    
    if( pick_keyPath(keyPath, m->keyPathVecStr) == 2 )
    {
        return (void*)this->m->ini.GetValue(m->keyPathVecStr[0].c_str(), m->keyPathVecStr[1].c_str(), nullptr);    // 成功返回a_pKey对应的值,即Section/key的值，反正返回nullptr
    }
    
    return nullptr;
}

int IniPs::cfg_add_arr(const char *keyPath)
{
    int ret = 0;
    return ret;
}

int IniPs::cfg_get_int(const char *keyPath, int &val)
{
    if( pick_keyPath(keyPath, m->keyPathVecStr) == 2 )
    {
        long ret = m->ini.GetLongValue(m->keyPathVecStr[0].c_str(), m->keyPathVecStr[1].c_str(), -99999999);
        if ( ret == -99999999 )
        {
            return -1;
        }

        val = (int)ret;
    }

    return 0;
}

int IniPs::cfg_get_str(const char *keyPath, string &val)
{
    const char* pGetVal = (const char*)cfg_get_val(keyPath);
    if ( pGetVal == nullptr )
    {
        return -1;
    }
    val = pGetVal;

    return 0;
}

int IniPs::cfg_get_bool(const char *keyPath, bool &val)
{
    /* 官方获取bool函数GetBoolValue()："t"、"y"、"on"、"1"开头为ture, "f"、"n"、"of"、"0"为false，先不用*/
    string str;
    if ( cfg_get_str(keyPath, str) == 0)
    {
        if ( str.compare("true") == 0 )
        {
            val = true;
            return 0;
        }
        if ( str.compare("false") == 0 )
        {
            val = false;
            return 0;
        }
    }

    return -1;
}

int IniPs::cfg_get_double(const char *keyPath, double &val)
{
    if( pick_keyPath(keyPath, m->keyPathVecStr) == 2 )
    {
        double ret = m->ini.GetDoubleValue(m->keyPathVecStr[0].c_str(), m->keyPathVecStr[1].c_str(), 99999999.9);
        
        if (  ret - 99999999.9 < 0.0001 && ret - 99999999.9 > -0.0001 )
        {
            return -1;
        }

        val = (double)ret;
    }

    return 0;
}

int IniPs::cfg_write()
{
    switch ( m->workMode )
    {
        case IniPs_s::FILEMODE:
            this->m->rc = this->m->ini.SaveFile(m->filePath.c_str());
            break;

        case IniPs_s::STRMODE:
            this->m->rc = this->m->ini.Save(this->m->IniStr);
            break;
        
        default:
            PRI_ERR("cfg_flush(): workMode is default");
            return -1;
    }
    if ( this->m->rc < SI_OK ) 
    {
        PRI_ERR("write %s error (%d)", (m->workMode == IniPs_s::FILEMODE) ? m->filePath.c_str() : m->IniStr.c_str(), this->m->rc);
        return -1;
    }

    cfg_flush();
    return 0;
}

int IniPs::cfg_set_int(const char *keyPath, const int &val)
{
    if( pick_keyPath(keyPath, m->keyPathVecStr) == 2 )
    {
        if ( this->m->ini.SetValue(m->keyPathVecStr[0].c_str(), m->keyPathVecStr[1].c_str(), to_string(val).c_str()) < SI_OK )
        {
            PRI_ERR("%s %d", keyPath, val);
            return -1;
        }

        if ( cfg_write() < 0 )
        {
            PRI_ERR("cfg_write()");
            return -1;
        }

        return 0;
    }

    return -1;
}

int IniPs::cfg_set_str(const char *keyPath, const string &val)
{
    if( pick_keyPath(keyPath, m->keyPathVecStr) == 2 )
    {
        if ( this->m->ini.SetValue(m->keyPathVecStr[0].c_str(), m->keyPathVecStr[1].c_str(), val.c_str()) < SI_OK )
        {
            PRI_ERR("%s %s", keyPath, val.c_str());
            return -1;
        }

        if ( cfg_write() < 0 )
        {
            PRI_ERR("cfg_write()");
            return -1;
        }

        return 0;
    }

    return -1;
}

int IniPs::cfg_set_bool(const char *keyPath, const bool &val)
{
    if( pick_keyPath(keyPath, m->keyPathVecStr) == 2 )
    {
        if ( this->m->ini.SetBoolValue(m->keyPathVecStr[0].c_str(), m->keyPathVecStr[1].c_str(), val) < SI_OK )
        {
            PRI_ERR("%s (bool)%d", keyPath, (int)val);
            return -1;
        }

        if ( cfg_write() < 0 )
        {
            PRI_ERR("cfg_write()");
            return -1;
        }

        return 0;
    }

    return -1;
}

int IniPs::cfg_set_double(const char *keyPath, const double &val)
{
    if( pick_keyPath(keyPath, m->keyPathVecStr) == 2 )
    {
        if ( this->m->ini.SetDoubleValue(m->keyPathVecStr[0].c_str(), m->keyPathVecStr[1].c_str(), val) < SI_OK )
        {
            PRI_ERR("%s %f", keyPath, val);
            return -1;
        }

        if ( cfg_write() < 0 )
        {
            PRI_ERR("cfg_write()");
            return -1;
        }

        return 0;
    }

    return -1;
}

int IniPs::cfg_add_int(const char *keyPath, const int &val)
{
    return cfg_set_int(keyPath, val);
}

int IniPs::cfg_add_str(const char *keyPath, const string &val)
{
    return cfg_set_str(keyPath, val);
}

int IniPs::cfg_add_bool(const char *keyPath, const bool &val)
{
    return cfg_set_bool(keyPath, val);
}

int IniPs::cfg_add_double(const char *keyPath, const double &val)
{
    return cfg_set_double(keyPath, val);
}

int IniPs::cfg_obj_iter(const char *keyPath, string &obj)
{
    int ret = -1;
    if( pick_keyPath(keyPath, m->keyPathVecStr) >= 1 )
    {
        std::list< CSimpleIniA::Entry > alce;
        if ( this->m->ini.GetAllKeys(m->keyPathVecStr[0].c_str(), alce) == false )
        {
            return -1;
        }

        obj.clear();
        obj.append("[").append(m->keyPathVecStr[0].c_str()).append("]\n");
        for(auto var : alce)
        {
            obj.append(var.pItem).append("=");
            obj.append(this->m->ini.GetValue(m->keyPathVecStr[0].c_str(), var.pItem, ""));
            obj.append("\n");
        }
        ret = obj.size();
    }

    return ret;
}

int IniPs::cfg_arr_iter(const char *keyPath, list<string>* obj)
{
    int ret = 0;
    int pickNum = 0;
    if( (pickNum = pick_keyPath(keyPath, m->keyPathVecStr)) >= 0 )
    {
        std::list< CSimpleIniA::Entry > alce;
        switch (pickNum)
        {
            case 0: // 获取全部的Section
                this->m->ini.GetAllSections(alce);
                break;

            case 1: // 获取全部的Key
                if ( this->m->ini.GetAllKeys(m->keyPathVecStr[0].c_str(), alce) == false )
                {
                    return -1;
                }
                break;

            case 2: // 获取全部的Key
                if ( this->m->ini.GetAllValues(m->keyPathVecStr[0].c_str(), m->keyPathVecStr[1].c_str(), alce) == false )
                {
                    return -1;
                }
                break;

            default:
                return -1;
        }

        if ( obj != nullptr )
        {
            obj->clear();
            for (auto &&ent : alce)
            {
                obj->push_back(ent.pItem);
            }
        }
        ret = alce.size();
    }

    return ret;
}

string IniPs::cfg_print_str()
{
    string ret;
    if ( this->get_init() == false )
    {
        return ret;
    }
    
    if( this->m->ini.Save(ret, true) <= SI_FAIL )
    {
        ret.clear();
    } 

    return ret;
}

int IniPs::cfg_save_file(const char* filePath)
{
    if ( this->m->ini.SaveFile(filePath) < SI_OK ) 
    {
        PRI_ERR("cfg_save_file %s error", filePath);
        return -1;
    }

    return 0;
}
