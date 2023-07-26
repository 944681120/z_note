#include "JsonPs.h"

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define     PRI(fmt, ...)  printf("[%s:%02d] "       fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)
#define PRI_INF(fmt, ...)  printf("[%s:%02d][INF]: " fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)
#define PRI_ERR(fmt, ...)  printf("[%s:%02d][ERR]: " fmt "\n", __FILENAME__, __LINE__, ##__VA_ARGS__)

using namespace LLZ;

int JsonPs::cfg_flush()
{
    if ( m->doc != nullptr )
    {
        yyjson_doc_free(m->doc);
        m->doc = nullptr;
    }
    
    if ( m->wdoc != nullptr )
    {
        yyjson_mut_doc_free(m->wdoc);
        m->wdoc = nullptr;
    }
    switch ( m->workMode )
    {
        case JsonPs_s::FILEMODE:      // json文件模式
            m->doc = yyjson_read_file(m->filePath.c_str(), m->rflg, NULL, &m->rerr);
            break;

        case JsonPs_s::STRMODE:       // json字符串模式
            m->doc = yyjson_read(m->jsonStr.c_str(), m->jsonStr.length(), 0);
            break;
        
        default:
            PRI_ERR("cfg_flush(): workMode is default");
            return -1;
    }
    
    if ( m->doc == nullptr )
    {
        PRI_ERR("read %s error(%u): %s at position: %ld", (m->workMode == JsonPs_s::FILEMODE) ? m->filePath.c_str() : m->jsonStr.c_str(), m->rerr.code, m->rerr.msg, m->rerr.pos);
        return -1;
    }
    
    m->wdoc = yyjson_doc_mut_copy(m->doc, NULL);
    return 0;
}

inline void JsonPs::cfg_flg_init()
{
    m->rflg = YYJSON_READ_ALLOW_COMMENTS | YYJSON_READ_ALLOW_TRAILING_COMMAS;
    m->wflg = YYJSON_WRITE_PRETTY | YYJSON_WRITE_ESCAPE_UNICODE;
}

int JsonPs::open(const char *filePathOrStr, int steLen)
{
    if ( steLen <= 0 )
    {
        m->filePath = filePathOrStr;
        m->workMode = JsonPs_s::FILEMODE;
    }
    else
    {
        m->jsonStr = filePathOrStr;
        m->workMode = JsonPs_s::STRMODE;
        if ( m->jsonStr.compare("") == 0 )
        {
            m->jsonStr = "{}";
        }
    }

    cfg_flg_init();
    cfg_flush();
    
    return 0;
}

JsonPs::JsonPs(const char *filePath)
{
    this->open(filePath);
}

JsonPs::JsonPs(const char *jsonStr, int strLen)
{
    this->open(jsonStr, strLen);
}

JsonPs::~JsonPs()
{
    if ( m->doc != nullptr )
    {
        yyjson_doc_free(m->doc);
        m->doc = nullptr;
    }
    
    if ( m->wdoc != nullptr )
    {
        yyjson_mut_doc_free(m->wdoc);
        m->wdoc = nullptr;
    }

    if ( this->m != nullptr )
    {
        delete this->m;
        this->m = nullptr;
    }
}

bool JsonPs::get_init()
{
    return (m->doc != nullptr);
}

void *JsonPs::cfg_get_val(const char *keyPath)
{
    if ( m->doc == nullptr || keyPath == nullptr )
    {
        return nullptr;
    }
    
    return yyjson_doc_ptr_get(m->doc, keyPath);
}

int JsonPs::cfg_add_arr(const char *keyPath)
{
    int ret = 0;
    if ( m->doc == nullptr || keyPath == nullptr )
    {
        return -1;
    }

    void* pRet = cfg_get_val(keyPath);
    if ( pRet != nullptr )
    {
        return 0;
    }

    yyjson_mut_doc *mwdoc = yyjson_mut_doc_new(nullptr);
    if ( !yyjson_mut_doc_ptr_set(m->wdoc, keyPath, yyjson_mut_arr(mwdoc)) || cfg_write() < 0)
    {
        PRI_ERR("yyjson_add_arr()");
        ret = -1;
    }

    if ( mwdoc != nullptr )
    {
        yyjson_mut_doc_free(mwdoc);
    }

    return ret;
}

int JsonPs::cfg_get_int(const char *keyPath, int &val)
{
    yyjson_val* pRet = (yyjson_val*)cfg_get_val(keyPath);
    if ( pRet == nullptr )
    {
        return -1;
    }

    if ( yyjson_is_int(pRet) == false )
    {
        return -1;
    }
    val = yyjson_get_int(pRet);
    return 0;
}

int JsonPs::cfg_get_str(const char *keyPath, string &val)
{
    yyjson_val* pRet = (yyjson_val*)cfg_get_val(keyPath);
    if ( pRet == nullptr )
    {
        return -1;
    }

    if ( yyjson_is_str(pRet) == false )
    {
        return -1;
    }
    val = yyjson_get_str(pRet);
    return 0;
}

int JsonPs::cfg_get_bool(const char *keyPath, bool &val)
{
    yyjson_val* pRet = (yyjson_val*)cfg_get_val(keyPath);
    if ( pRet == nullptr )
    {
        return -1;
    }

    if ( yyjson_is_bool(pRet) == false )
    {
        return -1;
    }
    val = yyjson_get_bool(pRet);
    return 0;
}

int JsonPs::cfg_get_double(const char *keyPath, double &val)
{
    yyjson_val* pRet = (yyjson_val*)cfg_get_val(keyPath);
    if ( pRet == nullptr )
    {
        return -1;
    }

    if ( yyjson_is_num(pRet) == false )
    {
        return -1;
    }
    val = yyjson_get_num(pRet);
    return 0;
}

int JsonPs::cfg_write()
{
    switch ( m->workMode )
    {
        case JsonPs_s::FILEMODE:
            yyjson_mut_write_file(m->filePath.c_str(), m->wdoc, m->wflg, NULL, &m->werr);
            break;

        case JsonPs_s::STRMODE:
            yyjson_mut_write_opts(m->wdoc, m->wflg, NULL, NULL, &m->werr);
            m->jsonStr = yyjson_mut_write(m->wdoc, 0, NULL);
            break;
        
        default:
            PRI_ERR("cfg_flush(): workMode is default");
            return -1;
    }
    if (m->werr.code) 
    {
        PRI_ERR("write %s error (%u): %s", (m->workMode == JsonPs_s::FILEMODE) ? m->filePath.c_str() : m->jsonStr.c_str(), m->werr.code, m->werr.msg);
        return -1;
    }

    cfg_flush();
    return 0;
}

int JsonPs::cfg_set_int(const char *keyPath, const int &val)
{
    if ( !yyjson_mut_doc_ptr_set(m->wdoc, keyPath, yyjson_mut_int(m->wdoc, val)) || cfg_write() < 0)
    {
        PRI_ERR("yyjson_set_int()");
        return -1;
    }
    return 0;
}

int JsonPs::cfg_set_str(const char *keyPath, const string &val)
{
    if ( !yyjson_mut_doc_ptr_set(m->wdoc, keyPath, yyjson_mut_str(m->wdoc, val.c_str())) || cfg_write() < 0)
    {
        PRI_ERR("yyjson_set_str()");
        return -1;
    }
    return 0;
}

int JsonPs::cfg_set_bool(const char *keyPath, const bool &val)
{
    if ( !yyjson_mut_doc_ptr_set(m->wdoc, keyPath, yyjson_mut_bool(m->wdoc, val)) || cfg_write() < 0)
    {
        PRI_ERR("yyjson_set_bool()");
        return -1;
    }
    return 0;
}

int JsonPs::cfg_set_double(const char *keyPath, const double &val)
{
    if ( !yyjson_mut_doc_ptr_set(m->wdoc, keyPath, yyjson_mut_real(m->wdoc, val)) || cfg_write() < 0)
    {
        PRI_ERR("yyjson_set_double()");
        return -1;
    }
    return 0;
}

int JsonPs::cfg_add_int(const char *keyPath, const int &val)
{
    void* pRet = cfg_get_val(keyPath);
    if ( pRet != nullptr )
    {
        return 0;
    }
    
    if ( !yyjson_mut_doc_ptr_add(m->wdoc, keyPath, yyjson_mut_int(m->wdoc, val)) || cfg_write() < 0)
    {
        PRI_ERR("yyjson_add_int()");
        return -1;
    }
    return 0;
}

int JsonPs::cfg_add_str(const char *keyPath, const string &val)
{
    void* pRet = cfg_get_val(keyPath);
    if ( pRet != nullptr )
    {
        return 0;
    }

    if ( !yyjson_mut_doc_ptr_add(m->wdoc, keyPath, yyjson_mut_str(m->wdoc, val.c_str())) || cfg_write() < 0)
    {
        PRI_ERR("yyjson_add_str()");
        return -1;
    }
    return 0;
}

int JsonPs::cfg_add_bool(const char *keyPath, const bool &val)
{
    void* pRet = cfg_get_val(keyPath);
    if ( pRet != nullptr )
    {
        return 0;
    }

    if ( !yyjson_mut_doc_ptr_add(m->wdoc, keyPath, yyjson_mut_bool(m->wdoc, val)) || cfg_write() < 0)
    {
        PRI_ERR("yyjson_add_bool()");
        return -1;
    }
    return 0;
}

int JsonPs::cfg_add_double(const char *keyPath, const double &val)
{
    void* pRet = cfg_get_val(keyPath);
    if ( pRet != nullptr )
    {
        return 0;
    }

    if ( !yyjson_mut_doc_ptr_add(m->wdoc, keyPath, yyjson_mut_real(m->wdoc, val)) || cfg_write() < 0)
    {
        PRI_ERR("yyjson_add_double()");
        return -1;
    }
    return 0;
}

int JsonPs::cfg_obj_iter(const char *keyPath, string& obj)
{
    yyjson_val* val = (yyjson_val*)cfg_get_val(keyPath);
    if ( val == nullptr )
    {
        return -1;
    }

    yyjson_mut_val* mval = yyjson_val_mut_copy( this->m->wdoc, val );
    yyjson_doc* mdoc = yyjson_mut_val_imut_copy(mval, NULL);
    if ( mdoc == nullptr )
    {
        return -1;
    }
    
    obj = yyjson_write(mdoc, 0, NULL);
    yyjson_doc_free(mdoc);
    return obj.size();
}

int JsonPs::cfg_arr_iter(const char *keyPath, list<string>* obj)
{
    int ret = 0;
    yyjson_val* val = nullptr;
    string pathStr(keyPath);
    if ( pathStr.compare("/") != 0 )
    {
        val = (yyjson_val*)cfg_get_val(keyPath);
        if ( val == nullptr )
        {
            return -1;
        }
    }
    
    /* 数组处理 */
    if ( yyjson_is_arr(val) )
    {
        string arriter;
        yyjson_arr_iter iarr = yyjson_arr_iter_with(val);
        while ((val = yyjson_arr_iter_next(&iarr))) 
        {
            if ( obj != nullptr )
            {
                cfg_obj_iter( (pathStr + "/" + to_string(ret)).c_str(), arriter);
                obj->push_back(arriter);
            }
            ret++;
        }
        return ret;
    }

    /* 非数组处理 */
    yyjson_obj_iter iter;
    if ( val == nullptr )   // "/"处理
    {
        iter = yyjson_obj_iter_with(yyjson_doc_get_root(this->m->doc));
    }
    else
    {
        iter = yyjson_obj_iter_with(val);
    }

    yyjson_val *key;
    while ((key = yyjson_obj_iter_next(&iter))) {
        if ( obj != nullptr )
        {
            obj->push_back(yyjson_get_str(key));
        }
        ret++;
    }

    return ret;
}

string JsonPs::cfg_print_str()
{
    string ret;
    if ( m->wdoc == nullptr )
    {
        return ret;
    }
    
    const char *json = yyjson_mut_write(m->wdoc, 0, NULL);
    if (json)
    {
        ret = json;
        free((void *)json);
    }
    return ret;
}

int JsonPs::cfg_save_file(const char *filePath)
{
    yyjson_mut_write_file(filePath, m->wdoc, m->wflg, NULL, &m->werr);
    if (m->werr.code) 
    {
        PRI_ERR("cfg_save_file %s error", filePath);
        return -1;
    }
    
    return 0;
}
