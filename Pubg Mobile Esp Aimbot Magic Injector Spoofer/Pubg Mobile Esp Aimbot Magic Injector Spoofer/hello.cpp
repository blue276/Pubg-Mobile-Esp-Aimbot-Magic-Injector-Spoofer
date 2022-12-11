#include "hello.h"


int exit_thread = 1;
int stop_thread = 0;
int safe = 1;
//FILE* f=NULL;
long * map_addr =NULL;

unsigned long mitux_addr=0;
unsigned long ow_addr = 0;
long poe_addr[100]={0};
int poe_count=0;



void * run_1(void*);
void* run_2(void*);
void* run_3(void*); //过闪退保护
void wait_connect();
void onRecv(int);
void seach_A_B();
void save_mitux_addr();
void read_data(float*,float*,float*);
void save_ow_addr();
void save_poe_addr();



int main()
{
 
    remove("/data/app/hello");
    
    char * game ="com.tencent.ig";
    
    if(!init(game))
        return 0;
    //f = fopen("/storage/emulated/0/测试.txt","w");
    
    //fprintf(f,"pid=%d--fd=%d\n",p_id,f_d);
    //fflush(f);
    
    
    //映射文件
    int fd = open("/storage/emulated/0/eme",O_CREAT|O_RDWR|O_TRUNC,0666);
    if(fd<=0)
        return -1;
        
    lseek(fd,4095,SEEK_SET);
    write(fd,"\0",1);
    map_addr =(long*)mmap(NULL,4095,PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
    close(fd);
    
    if(map_addr ==NULL)
    {
        
        return -1;
    }
    
    memset(map_addr,0,4096);
        
    //fprintf(f,"map=%x\n",map_addr);
   // fflush(f);
    
    
    pthread_t thread1=0,thread2=0,thread3=0;
    
    pthread_create(&thread1,NULL,run_1,NULL);
    
    pthread_detach(thread1);
    
    pthread_create(&thread2,NULL,run_2,NULL);
    
    pthread_detach(thread2);
    
    pthread_create(&thread3,NULL,run_3,NULL);
    
    pthread_detach(thread3);
    
    
	
    wait_connect();
    
   // fprintf(f,"退出\n");
    
    if(map_addr !=NULL)
    {
        munmap((void*)map_addr,4096);
        map_addr=NULL;
    }
    
    //fclose(f);
    destory();
    
   
    return 0;
}

//读取数据
void* run_1(void*)
{
    //fprintf(f,"线程1运行\n");
    //fflush(f);
    
    //矩阵
    float matx[16]={0};
    float owxyz[3]={0};
    float poexyz[3]={0};
    
    while(exit_thread)
    {
        if(stop_thread==1)
        {
            read_data(matx,owxyz,poexyz);
            sleep(0.5);
        }else
        {
            sleep(3);
        }
        
        
    }
    
    
    return NULL;
}

//读取矩阵
void read_data(float *mtx,float* owx,float* poe)
{
    if(mitux_addr<=0 || map_addr==NULL ||poe_addr[99]<=0)
    {
        memset(map_addr,0,4096);
        return;
    }
        
    ReadProcessMemonyValue_Float(mitux_addr,212,mtx,64);
    ReadProcessMemonyValue_Float(ow_addr,32,owx,12);
    
    ((long*)map_addr)[1010]=owx[0];
    ((long*)map_addr)[1011]=owx[1];
    ((long*)map_addr)[1012]=owx[2];
    
    ((float*)map_addr)[700]=mtx[0];
    ((float*)map_addr)[701]=mtx[1];
    ((float*)map_addr)[702]=0.0;
    ((float*)map_addr)[703]=mtx[3];
    ((float*)map_addr)[704]=mtx[4];
    ((float*)map_addr)[705]=mtx[5];
    ((float*)map_addr)[706]=0.0;
    ((float*)map_addr)[707]=mtx[7];
    ((float*)map_addr)[708]=0.0;
    ((float*)map_addr)[709]=mtx[9];
    ((float*)map_addr)[710]=0.0;
    ((float*)map_addr)[711]=mtx[11];
    ((float*)map_addr)[712]=mtx[12];
    ((float*)map_addr)[713]=mtx[13];
    ((float*)map_addr)[714]=mtx[14];
    ((float*)map_addr)[715]=mtx[15];
    ((long*)map_addr)[1022]=poe_addr[99];
    
    
    int count = poe_addr[99];
    
    int i,j,k;
    unsigned long add=0;
    //float hp = 0;
    
    for(i=0,j=0,k=0;i<=count;i++,j++,k=j*4)
    {
        if(poe_addr[i]==0)
            continue;
        add =(unsigned long)(poe_addr[i]);
        ReadProcessMemonyValue_Float(add,32,poe,12);
        
       /* if(poe[0]!=0.0 || poe[4]!=0.0)
            continue;*/
      
      //  ReadProcessMemonyValue_Float(add,-1944,&hp,4);
        ((long*)map_addr)[k]=poe[0];
        ((long*)map_addr)[k+1]=poe[1];
        ((long*)map_addr)[k+2]=poe[2];
        //((long*)map_addr)[k+3]=hp;
              
    }
    
    
}

//获取敌人数据
void * run_2(void*)
{
   // fprintf(f,"线程2运行\n");
    //fflush(f);
    long poe[]={282,56,512,76,1321122848};
    while(exit_thread)
    {
        if(stop_thread==1)
        {
            poe_count = 0;
            ReadProcess_Memony(poe,6);
            save_poe_addr();
            sleep(10);
        }else
        {
            sleep(3);
        }
    }
    
    return NULL;
}


void * run_3(void*)
{
    //fprintf(f,"线程3运行\n");
    //fflush(f);
    
    while(safe)
    {
        ReadProcessMemonySafe();
    }
    
    return NULL;
}





void wait_connect()
{
    
    int socket_fd =-1,connect_fd=-1;
    
    struct sockaddr_in seraddr;
    int port =6585;
    int res = 1;
    int ty = 0;
    
    
    
    
    socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if(socket_fd<=0)
    {
       // fprintf(f,"create socket error\n");
       // fflush(f);
        return;
    }
    
    memset(&seraddr,0,sizeof(seraddr));
    
    seraddr.sin_family=AF_INET;
    seraddr.sin_port=htons(port);
    seraddr.sin_addr.s_addr=htonl(INADDR_ANY);
    setsockopt(socket_fd,SOL_SOCKET,SO_REUSEADDR,&res,sizeof(res));
    
    bind(socket_fd,(struct sockaddr*)&seraddr,sizeof(seraddr));
    
    listen(socket_fd,10);
    
   // fprintf(f,"load…\n");
   // fflush(f);
    
    
    while(exit_thread)
    {
        if((connect_fd = accept(socket_fd,NULL,NULL))==-1)
            continue;
            
        onRecv(connect_fd);
        
        close(connect_fd);
        
        //fprintf(f,"连接断开\n");
        //fflush(f);
    }
    
    close(socket_fd);
}

void onRecv(int con)
{
    
    
    int ty =0;
    if(recv(con,(char*)&ty,sizeof(ty),0)<=0)
    {
        return ;
    }
    
    switch(ty)
    {
        case REQ_MAPS:
            stop_thread = 0;
            safe = 0;
            poe_count=0;
            seach_A_B();
            stop_thread = 1;
            break;
        case REQ_EXIT:
           stop_thread=0;
           exit_thread=0;
           safe=0;
            break;
        case REQ_STOP:
            stop_thread = 0;
            safe = 0;
            break;
        case REQ_INIT:
            break;
        
    }
    
}

void seach_A_B()
{
    //矩阵
    long mitux[]={1051372203,952,-2147483648,-8,1051372203};
    ReadProcess_Memony(mitux,6);
    save_mitux_addr();
    
    //自己坐标
    long ow[]={282,120,258,12,1321122848};
    ReadProcess_Memony(ow,6);
    save_ow_addr();
    
}

//提取矩阵基质
void save_mitux_addr()
{
    FILE * fp = fopen(AS,"r");
    if(fp==NULL)
        return;
    char line[32]={0};
    if(fgets(line,sizeof(line),fp)!=0)
    {
        sscanf(line,"%u",&mitux_addr);
    }
    fclose(fp);
}

//提取自己坐标地址
void save_ow_addr()
{
    FILE * fp = fopen(AS,"r");
    if(fp==NULL)
        return;
    char line[32]={0};
    if(fgets(line,sizeof(line),fp)!=0)
    {
        sscanf(line,"%u",&ow_addr);
    }
    fclose(fp);
    
}

//提取敌人基质
void save_poe_addr()
{
    FILE * fp = fopen(AS,"r");
    if(fp==NULL)
        return;
    char line[32]={0};
    unsigned long base = 0;
    while(fgets(line,sizeof(line),fp)!=0)
    {
        if(sscanf(line,"%u",&base)>=1)
        {
            if(ow_addr==base)
                continue;
            poe_count++;
            poe_addr[99]=poe_count;
            poe_addr[poe_count]=base;
        }
    }
    
    fclose(fp);
}

