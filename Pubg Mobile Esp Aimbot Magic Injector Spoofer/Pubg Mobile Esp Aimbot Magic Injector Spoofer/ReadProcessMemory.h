#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>  
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

static int p_id=-1;
static int f_d;





//内存内型
enum
{
    REGION_JH=1,//   1    java 堆 rw p[/dev/ashmem/dalvik- 包含 main large allocation]
    REGION_CH,//     2    c++ 堆 [heap]
    REGION_CA,//     3    c++自动分配堆 [anon:libc_malloc]
    REGION_CD,//     4    c++数据段 [/data/app/com.] [.so]
    REGION_CB,//     5    C++ BSS段，未初始化全局变量  [anon:.bss]
    REGION_ANONYMOUS,// 6  匿名内存
    REGION_JAVA,//      7  java段
    REGION_STACK,//     8  栈[stack]
    REGION_ASHMEM,//    9  匿名共享段
    REGION_OTHER,//     10 其他段
    REGION_BAD,//       11 
    REGION_XA,//        12 app代码段
    REGION_XS,//        13 系统代码段
    REGION_ALL=0,
};

#define JH "/storage/emulated/0/jh.db"
#define CH "/storage/emulated/0/ch.db"
#define CA "/storage/emulated/0/ca.db"
#define CD "/storage/emulated/0/cd.db"
#define CB "/storage/emulated/0/cb.db"
#define STACK "/storage/emulated/0/stack.db"
#define AS "/storage/emulated/0/anonymous.db"
#define JAVA "/storage/emulated/0/java.db"
#define BAD "/storage/emulated/0/bad.db"
#define XA "/storage/emulated/0/xa.db"
#define XS "/storage/emulated/0/xs.db"











/*
*声明函数
*
*/
void read_map_jh(long*);//解析jh
void read_map_ch(long*);//解析ch
void read_map_ca(long*);//解析ca
void read_map_cd(long*);//解析cd
void read_map_cb(long*);//解析cb
void read_map_stack(long*);//解析栈
void read_map_as(long*);//解析a内存
void read_map_ja(long*);//解析java内存
void read_map_vb(long*);//解析b内存
void read_map_xa(long*);//解析xa内存
void read_map_xs(long*);//解析xs内存
void ReadProcess_Memony(long*,int);//搜索特征码
void memory_search_dword(long*,unsigned long,unsigned long,FILE*);//搜索特征码
int get_pid(const char*);//获取pid
bool init();//初始化
void destory();//销毁

void ReadProcessMemonyValue_Dword(unsigned long addr,long offset,long* buff,long size);
void ReadProcessMemonyValue_Float(unsigned long addr,long offset,float* buff,long size);
void ReadProcessMemonyValue_Byte(unsigned long addr,long offset,char*buff,long size);
void WriteProcessMemonyValue_Dword(unsigned long addr,long offset,long*data,long size);
void WriteProcessMemonyValue_Float(unsigned long addr,long offset,float*data,long size);

void ReadProcessMemonySafe();//过游戏闪退保护
void memory_search_dword_safe(unsigned long,unsigned long);//过闪退
void read_map_a();//过闪退
/*
*bm表示包名
*
*/
bool init(const char* bm)
{
    char mem[32];
    p_id = get_pid(bm);
    if(p_id<=0)
        return false;
    sprintf(mem,"/proc/%d/mem",p_id);
    f_d = open(mem,O_RDWR);
    if(f_d<=0)
        return false;
    return true;
}

//销毁
void destory()
{
    close(f_d);
    remove(JH);
    remove(CH);
    remove(CA);
    remove(CB);
    remove(CD);
    remove(STACK);
    remove(AS);
    remove(JAVA);
    remove(BAD);
    remove(XA);
    remove(XS);
}




//根据包名获取进程pid
int get_pid(const char *process_name)    
{    
    int id=0;    
    pid_t pid = -1;    
    DIR* dir;    
    FILE *fp;    
    char filename[32]={0};    
    char cmdline[256]={0};    
    
    struct dirent * entry;    
    
    if (process_name == NULL)    
        return -1;    
    
    dir = opendir("/proc");    
    if (dir == NULL)    
        return -1;    
    
    while((entry = readdir(dir)) != NULL) {    
        id = atoi(entry->d_name);    
        if (id != 0) {    
            sprintf(filename, "/proc/%d/cmdline", id);    
            fp = fopen(filename, "r");    
            if (fp) {    
                fgets(cmdline, sizeof(cmdline), fp);    
                fclose(fp);    
    
                if (strcmp(process_name, cmdline) == 0)
                {    
                    /* process found */    
                    pid = id;    
                    break;    
                }    
            }    
        }    
    }    
    
    closedir(dir);    
    return pid;    
}    

//过游戏闪退保护
void ReadProcessMemonySafe()
{
    read_map_a();
}





/*
*addr表示要读取地址
*
*offset表示偏移
*buff表示存储读取数据
*size表示读取字节数
*/
void ReadProcessMemonyValue_Dword(unsigned long addr,long offset,long* buff,long size)
{
    do
    {
        if(buff==NULL)
            break;
        if(size==0)
            break;
        if(addr<=0)
            break;
        if(f_d==-1)
            break;
            pread64(f_d,buff,size,addr+offset);
    }while(false);
}

void ReadProcessMemonyValue_Float(unsigned long addr,long offset,float* buff,long size)
{
    do
    {
        if(buff==NULL)
            break;
        if(size==0)
            break;
        if(addr<=0)
            break;
        if(f_d==-1)
            break;
            pread64(f_d,buff,size,addr+offset);
    }while(false);
}

void ReadProcessMemonyValue_Byte(unsigned long addr,long offset,char*buff,long size)
{
    do
    {
        if(buff==NULL)
            break;
        if(size==0)
            break;
        if(addr<=0)
            break;
        if(f_d==-1)
            break;
            pread64(f_d,buff,size,addr+offset);
    }while(false);
}

/*
*addr表示写入地址
*offset表示偏移
*data 表示写入的数据
*size表示写入自节数
*/
void WriteProcessMemonyValue_Dword(unsigned long addr,long offset,long*data,long size)
{
    do
    {
        if(data==NULL)
            break;
        if(size==0)
            break;
        if(addr<=0)
            break;
        if(f_d==-1)
            break;
            pwrite64(f_d,data,size,addr+offset);
    }while(false);
}

void WriteProcessMemonyValue_Float(unsigned long addr,long offset,float*data,long size)
{
    do
    {
        if(data==NULL)
            break;
        if(size==0)
            break;
        if(addr<=0)
            break;
        if(f_d==-1)
            break;
            pwrite64(f_d,data,size,addr+offset);
    }while(false);
}


/*
*ReadProcess_Memony()
*
*data表示要搜索的特征码
*type表示搜索内存类型
*
*/
//读取进程内存特征码
void ReadProcess_Memony(long *data,int type)
{
    do
    {
        if(f_d==-1)
            break;
            switch(type)
            {
                case REGION_JH:
                    read_map_jh(data);
                    break;
                case REGION_CH:
                    read_map_ch(data);
                    break;
                case REGION_CA:
                    read_map_ca(data);
                    break;
                case REGION_CD:
                    read_map_cd(data);
                    break;
                case REGION_CB:
                    read_map_cb(data);
                    break;
                case REGION_ANONYMOUS:
                    read_map_as(data);
                    break;
                case REGION_JAVA:
                    read_map_ja(data);
                    break;
                case REGION_STACK:
                    read_map_stack(data);
                    break;
                case REGION_ASHMEM:
                    read_map_ja(data);
                    break;
                case REGION_OTHER:
                    //
                    break;
                case REGION_BAD:
                    read_map_vb(data);
                    break;
                case REGION_XA:
                    read_map_xa(data);
                    break;
                case REGION_XS:
                    read_map_xs(data);
                    break;
            }
        
        
        
    }while(false);
}
void read_map_xs(long* data)
{
    char path[32];
    char buff[256];
    unsigned long start=0;
    unsigned long end=0;
    
    FILE *fp=NULL;
    FILE *file=NULL;
    
    do
    {
        sprintf(path,"/proc/%d/maps",p_id);
        fp = fopen(path,"r");
        if(fp==NULL)
            break;
            file=fopen(XS,"w");
            while(!feof(fp))
            {
                fgets(buff,sizeof(buff),fp);
                if((strstr(buff,"r-xp")!=NULL) && strstr(buff,"/system/"))
                {
                    sscanf(buff,"%lx-%lx",&start,&end);
                    memory_search_dword(data,start,end,file);
                    //printf("%x-%x\n",start,end);
                }
            }
            fclose(fp);
            fclose(file);
        
        
    }while(false);
}


void read_map_xa(long* data)
{
    char path[32];
    char buff[256];
    unsigned long start=0;
    unsigned long end=0;
    
    FILE *fp=NULL;
    FILE *file=NULL;
    
    do
    {
        sprintf(path,"/proc/%d/maps",p_id);
        fp = fopen(path,"r");
        if(fp==NULL)
            break;
            file=fopen(XA,"w");
            while(!feof(fp))
            {
                fgets(buff,sizeof(buff),fp);
                if((strstr(buff,"r-xp")!=NULL) && strstr(buff,"/data/app/"))
                {
                    sscanf(buff,"%lx-%lx",&start,&end);
                    memory_search_dword(data,start,end,file);
                    //printf("%x-%x\n",start,end);
                }
                
            }
            fclose(fp);
            fclose(file);
        
    }while(false);
}


void read_map_vb(long* data)
{
    char path[32];
    char buff[256];
    unsigned long start=0;
    unsigned long end=0;
    
    FILE *fp=NULL;
    FILE *file=NULL;
    
    do
    { 
        sprintf(path,"/proc/%d/maps",p_id);
        fp = fopen(path,"r");
        if(fp==NULL)
            break;
            file=fopen(BAD,"w");
            while(!feof(fp))
            {
                fgets(buff,sizeof(buff),fp);
                if((strstr(buff,"rw-s")!=NULL) && strstr(buff,"kgsl-3d0"))
                {
                    sscanf(buff,"%lx-%lx",&start,&end);
                    memory_search_dword(data,start,end,file);
                    //printf("%x-%x\n",start,end);
                }
            }
            fclose(fp);
            fclose(file);
        
        
    }while(false);
}






void read_map_ja(long* data)
{
    char path[32];
    char buff[256];
    unsigned long start=0;
    unsigned long end=0;
    
    FILE *fp=NULL;
    FILE *file=NULL;
    
    do
    {
        sprintf(path,"/proc/%d/maps",p_id);
        fp = fopen(path,"r");
        if(fp==NULL)
            break;
            file=fopen(JAVA,"w");
            
            while(!feof(fp))
            {
                fgets(buff,sizeof(buff),fp);
                if((strstr(buff,"rw-p")!=NULL) && (strstr(buff,"/dev/ashmem/")) && strstr(buff,"dalvik"))
                {
                     sscanf(buff,"%lx-%lx",&start,&end);
                     memory_search_dword(data,start,end,file);
                    // printf("%x-%x\n",start,end);
                }
                
            }
            fclose(fp);
            fclose(file);
        
    }while(false);
}



void read_map_as(long* data)
{
    char path[32];
    char buff[256];
    unsigned long start=0;
    unsigned long end=0;
    
    FILE *fp=NULL;
    FILE *file=NULL;
    
    do
    {
        sprintf(path,"/proc/%d/maps",p_id);
        fp = fopen(path,"r");
        if(fp==NULL)
            break;
            file=fopen(AS,"w");
            while(!feof(fp))
            {
                 fgets(buff,sizeof(buff),fp);
                 if((strstr(buff,"rw-p")!=NULL) && (strlen(buff)<42))
                 {
                     sscanf(buff,"%lx-%lx",&start,&end);
                     memory_search_dword(data,start,end,file);
                     //printf("%x-%x\n",start,end);
                 }
                 
            }
            fclose(fp);
            fclose(file);
        
    }while(false);
    
}





void read_map_stack(long* data)
{
    char path[32];
    char buff[256];
    unsigned long start=0;
    unsigned long end=0;
    
    FILE *fp=NULL;
    FILE *file=NULL;
    
    do
    {
        sprintf(path,"/proc/%d/maps",p_id);
        fp = fopen(path,"r");
        if(fp==NULL)
            break;
            file=fopen(STACK,"w");
            while(!feof(fp))
            {
                fgets(buff,sizeof(buff),fp);
                if((strstr(buff,"rw-p")!=NULL) && strstr(buff,"[stack]"))
                {
                    sscanf(buff,"%lx-%lx",&start,&end);
                    memory_search_dword(data,start,end,file);
                    //printf("%x-%x\n",start,end);
                }
            }
            fclose(fp);
            fclose(file);
    }while(false);
    
}



void read_map_cb(long* data)
{
    char path[32];
    char buff[256];
    unsigned long start=0;
    unsigned long end=0;
    
    FILE *fp=NULL;
    FILE *file=NULL;
    
    do
    {
        sprintf(path,"/proc/%d/maps",p_id);
        fp = fopen(path,"r");
        if(fp==NULL)
            break;
            file=fopen(CB,"w");
            while(!feof(fp))
            {
                fgets(buff,sizeof(buff),fp);
                if((strstr(buff,"rw-p")!=NULL) && strstr(buff,"[anon:.bss]"))
                {
                    sscanf(buff,"%lx-%lx",&start,&end);
                    memory_search_dword(data,start,end,file);
                }
            }
            fclose(fp);
            fclose(file);
        
        
    }while(false);
    
    
}




void read_map_cd(long* data)
{
    char path[32];
    char buff[256];
    unsigned long start=0;
    unsigned long end=0;
    
    FILE *fp=NULL;
    FILE *file=NULL;
    
    do
    {
        sprintf(path,"/proc/%d/maps",p_id);
        fp = fopen(path,"r");
        if(fp==NULL)
            break;
            file=fopen(CD,"w");
            
            while(!feof(fp))
            {
                fgets(buff,sizeof(buff),fp);
                if((strstr(buff,"rw-p")!=NULL) && strstr(buff,"/data/app/") && strstr(buff,".so"))
                {
                    sscanf(buff,"%lx-%lx",&start,&end);
                    memory_search_dword(data,start,end,file);
                    //printf("%x-%x\n",start,end);
                    
                }
            }
            fclose(fp);
            fclose(file);
        
    }while(false);
    
}




void read_map_ca(long* data)
{
    char path[32];
    char buff[256];
    unsigned long start=0;
    unsigned long end=0;
    
    FILE *fp=NULL;
    FILE *file=NULL;
    
    do
    {
        sprintf(path,"/proc/%d/maps",p_id);
        fp = fopen(path,"r");
        if(fp==NULL)
            break;
            file=fopen(CA,"w");
            while(!feof(fp))
            {
                fgets(buff,sizeof(buff),fp);
                if((strstr(buff,"rw")!=NULL) && strstr(buff,"[anon:libc_malloc]"))
                {
                    sscanf(buff,"%lx-%lx",&start,&end);
                    memory_search_dword(data,start,end,file);
                    
                }
                
                
            }
            fclose(fp);
            fclose(file);
        
        
    }while(false);
    
}






//搜索ch
void read_map_ch(long*data)
{
    char path[32];
    char buff[256];
    unsigned long start=0;
    unsigned long end=0;
    
    FILE *fp=NULL;
    FILE *file=NULL;
    
    do
    {
        sprintf(path,"/proc/%d/maps",p_id);
        fp = fopen(path,"r");
        if(fp==NULL)
            break;
            file=fopen(CH,"w");
            while(!feof(fp))
            {
                fgets(buff,sizeof(buff),fp);
                if((strstr(buff,"rw-p")!=NULL) && (strstr(buff,"[heap]")))
                {
                    sscanf(buff,"%lx-%lx",&start,&end);
                    memory_search_dword(data,start,end,file);
                    
                }
                
                
            }
            fclose(fp);
            fclose(file);
        
    }while(false);
}





/*
*读取maps文件
*
*data代表被搜索特征码
*/
void read_map_jh(long* data)
{
    char path[32];
    char buff[256];
    unsigned long start=0;
    unsigned long end=0;
    
    FILE *fp=NULL;
    FILE *file=NULL;
    
    do
    {
        sprintf(path,"/proc/%d/maps",p_id);
        fp = fopen(path,"r");
        if(fp==NULL)
            break;
            
           file=fopen(JH,"w");
            while(!feof(fp))
            {
                fgets(buff,sizeof(buff),fp);
                if((strstr(buff,"rw-p")!=NULL) && (strstr(buff,"/dev/ashmem/")))
                {
                    sscanf(buff,"%lx-%lx",&start,&end);
                    memory_search_dword(data,start,end,file);
                    
                    
                }
            }
            fclose(fp);
            fclose(file);
            
        
    }while(false);
    
    
}

/*
*
*data[0]代表主特征码 data[1]表示偏移量 data[2]表示偏移量的值 data[3]表示偏移量 data[4]表示偏移量的值
*start表示起始地址
*end表示结束地址
*file表示存放搜索到地址
*/
void memory_search_dword(long* data,unsigned long start,unsigned long end,FILE*file)
{
    unsigned long lenth = (end-start)/4096;
   // int len = (data[2]/4)-1;
    long buff[1024];
    long value_1=0;
    long value_2=0;
    
    
    for(int i=0;i<lenth;++i)
    {
        pread64(f_d,buff,4096,start+(i*4096));
        
        for(int j=0;j<1024;++j)
        {
            if(buff[j]==data[0])
            {
                //printf("%x\n",start+(i*4096)+(j*4));
                
                pread64(f_d,&value_1,4,start+(i*4096)+(j*4)+data[1]);
                pread64(f_d,&value_2,4,start+(i*4096)+(j*4)+data[1]+data[3]);
                
                if(value_1== data[2] && value_2==data[4])
                {
                    fprintf(file,"%u\n",start+(i*4096)+(j*4));
                    fflush(file);
                }
                
            }
        }
    }
}


//搜索a内存过保护
void read_map_a()
{
    char path[32];
    char buff[256];
    unsigned long start=0;
    unsigned long end=0;
    
    FILE *fp=NULL;
    //FILE *file=NULL;
    
    do
    {
        sprintf(path,"/proc/%d/maps",p_id);
        fp = fopen(path,"r");
        if(fp==NULL)
            break;
            //file=fopen(AS,"w");
            while(!feof(fp))
            {
                 fgets(buff,sizeof(buff),fp);
                 if((strstr(buff,"rw-p")!=NULL) && (strlen(buff)<42))
                 {
                     sscanf(buff,"%lx-%lx",&start,&end);
                     memory_search_dword_safe(start,end);
                     
                 }
                 
            }
            fclose(fp);
            //fclose(file);
        
    }while(false);
    
}

//搜索内存过保护
void memory_search_dword_safe(unsigned long start,unsigned long end)
{
    unsigned long lenth = (end-start)/4096;
   // int len = (data[2]/4)-1;
    long buff[1024];
    
    
    for(int i=0;i<lenth;++i)
    {
        pread64(f_d,buff,4096,start+(i*4096));
    }
}
