







#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#define kapasite 24

struct siralama{
	int oncelik_id;
	int ucak_id;
	int talep_edilen_inis_saati;
	int inis_saati;
	int kalkis_saati;
	int gecikme_suresi;
};

struct siralama *gelenUcak=NULL;
int front=-1;
int rear=-1;
int boyut=2;

struct inis_pisti_kullanim_sirasi{
    int oncelik_id;
	int ucak_id;
	int inis_saati;
	int gecikme_suresi;
	struct inis_pisti_kullanim_sirasi *next;
};

struct inis_pisti_kullanim_sirasi *head=NULL;

struct kalkis_pisti_kullanim_sirasi{
    int oncelik_id;
	int ucak_id;
	int kalkis_saati;
	struct kalkis_pisti_kullanim_sirasi *next;
};

struct kalkis_pisti_kullanim_sirasi *bas=NULL;

bool bos_mu()
{
	return front==rear;
}

bool dolu_mu()
{
	return rear==kapasite-1;
}

void enqueue(int,int,int);
int dequeue(struct siralama *,int,struct siralama,int);
void enqueue_inis();
void enqueue_kalkis();
void inis_uygun_mu();
void oncelikli_yap();
void dosyadan_oku();
void output_guncelle();

int main()
{
	dosyadan_oku();
	printf("Tum talepler degerlendirildi. Output dosyasi hazir.\n");
	return 0;
}

void dosyadan_oku()
{
    FILE *dosya;
	dosya=fopen("input.txt","r");
	fseek(dosya,44,SEEK_SET);

	int oncelik,ucak,saat;
	while(!feof(dosya))
	{
		fscanf(dosya,"%d %d %d",&oncelik,&ucak,&saat);
		printf("\nYeni satir okundu..\n");
		printf("Okunan veriler: %d %d %d\n",oncelik,ucak,saat);
		enqueue(oncelik,ucak,saat);
		inis_uygun_mu();
		oncelikli_yap();

		printf("\nTalep degerlendirildi.\nOutput dosyasi guncellendi.\n\n");
        output_guncelle();
        enqueue_inis();
        enqueue_kalkis();
        printf("Devam etmek icin bir tusa basiniz.");
        getchar();
        system("CLS");
	}
	fclose(dosya);
}

void enqueue(int oncelik,int ucak,int saat)
{
	if(dolu_mu())
	{
		printf("Kuyruk dolu!\n");
	}
	if(front==-1)
	{
		front=0;
		rear+=1;
	}
	if(gelenUcak==NULL)
	{
		gelenUcak=(struct siralama*)malloc(sizeof(struct siralama)*2);
	}
	if(rear>=boyut)
	{
		int i;
		boyut*=2;
		struct siralama *gelenUcak2=(struct siralama*)malloc(sizeof(struct siralama)*boyut);
		for(i=0;i<boyut/2;i++)
			gelenUcak2[i]=gelenUcak[i];
		free(gelenUcak);
		gelenUcak=gelenUcak2;
	}
	if(saat<=24 && saat+1<=24)
	{
		gelenUcak[rear].oncelik_id=oncelik;
		gelenUcak[rear].ucak_id=ucak;
		gelenUcak[rear].talep_edilen_inis_saati=saat;
		gelenUcak[rear].inis_saati=saat;
		gelenUcak[rear].kalkis_saati=saat+1;
		gelenUcak[rear].gecikme_suresi=0;
		rear++;
	}
}

int dequeue(struct siralama *dizi,int size,struct siralama temp,int konum)
{
	if(bos_mu())
	{
		printf("Kuyruk bos!\n");
	}
	int i=konum;
	int j;
	if(i<size)
	{
		for(j=i;j<size;j++)
		{
			dizi[j]=dizi[j+1];
		}
		size=size-1;
	}
	return size;
}

void enqueue_inis()
{
    int i;
    struct inis_pisti_kullanim_sirasi *inis=head;
    printf("Guncel inis kuyrugu:\n");
    printf("oncelik_id  ucak_id  inis_saati  gecikme_suresi\n");
    for(i=front;i<rear;i++)
    {
        inis=(struct inis_pisti_kullanim_sirasi*)malloc(sizeof(struct inis_pisti_kullanim_sirasi));
        inis->oncelik_id=gelenUcak[i].oncelik_id;
        inis->ucak_id=gelenUcak[i].ucak_id;
        inis->inis_saati=gelenUcak[i].inis_saati;
        inis->gecikme_suresi=gelenUcak[i].gecikme_suresi;
        printf("%5d %10d %10d %10d\n",inis->oncelik_id,inis->ucak_id,inis->inis_saati,inis->gecikme_suresi);
        inis=inis->next;
    }
    printf("\n");
}

void enqueue_kalkis()
{
    int i;
    struct kalkis_pisti_kullanim_sirasi *kalkis=bas;
    printf("Guncel kalkis kuyrugu:\n");
    printf("oncelik_id  ucak_id  kalkis_saati\n");
    for(i=front;i<rear;i++)
    {
        kalkis=(struct kalkis_pisti_kullanim_sirasi*)malloc(sizeof(struct kalkis_pisti_kullanim_sirasi));
        kalkis->oncelik_id=gelenUcak[i].oncelik_id;
        kalkis->ucak_id=gelenUcak[i].ucak_id;
        kalkis->kalkis_saati=gelenUcak[i].kalkis_saati;
        printf("%5d %10d %10d\n",kalkis->oncelik_id,kalkis->ucak_id,kalkis->kalkis_saati);
        kalkis=kalkis->next;
    }
    printf("\n");
}

void inis_uygun_mu()
{
    int i,j,x,y;
	if(rear<24)
	{
		for(i=rear-1;i>=front;i--)
        {
            for(j=front;j<rear;j++)
            {
                if(gelenUcak[i].inis_saati==gelenUcak[j].inis_saati)
                {
                    if(gelenUcak[i].oncelik_id > gelenUcak[j].oncelik_id)
                    {
                        if(gelenUcak[i].gecikme_suresi<3 && gelenUcak[j].gecikme_suresi<3)
                        {
                            // i ertelenecek
                            gelenUcak[i].inis_saati += 1;
                            gelenUcak[i].kalkis_saati += 1;
                            gelenUcak[i].gecikme_suresi++;
                            if(gelenUcak[i].inis_saati<=24 && gelenUcak[i].kalkis_saati<=24)
                            {
                                printf("%d %d %d degerli ucak talep edilen saatinin dolu olmasi sebebi %d saatine ertelenmistir.\n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati,gelenUcak[i].inis_saati);
                                inis_uygun_mu();
                            }
                            else
                            {
                                printf("%d %d %d degerli ucak gun icinde inis yapamadigi icin iptal edilmistir.\n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati);
                                int sil=dequeue(gelenUcak,rear,gelenUcak[i],i);
                                rear=sil;
                            }
                        }
                        else if(gelenUcak[i].gecikme_suresi<3 && gelenUcak[j].gecikme_suresi==3)
                        {
                            // i ertelenecek
                            gelenUcak[i].inis_saati += 1;
                            gelenUcak[i].kalkis_saati += 1;
                            gelenUcak[i].gecikme_suresi++;
                            if(gelenUcak[i].inis_saati<=24 && gelenUcak[i].kalkis_saati<=24)
                            {
                                printf("%d %d %d degerli ucak talep edilen saatinin dolu olmasi sebebi %d saatine ertelenmistir.\n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati,gelenUcak[i].inis_saati);
                                inis_uygun_mu();
                            }
                            else
                            {
                                printf("%d %d %d degerli ucak gun icinde inis yapamadigi icin iptal edilmistir.\n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati);
                                int sil=dequeue(gelenUcak,rear,gelenUcak[i],i);
                                rear=sil;
                            }
                        }
                        else if(gelenUcak[i].gecikme_suresi==3 && gelenUcak[j].gecikme_suresi<3)
                        {
                            // i ertelenemiyor j ertelenecek
                            printf("%d %d %d degerli ucak 3 kez ertelendigi icin %d saatinde oncelikli olarak indirilmistir.\n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati,gelenUcak[i].inis_saati);
                            gelenUcak[j].inis_saati += 1;
                            gelenUcak[j].kalkis_saati += 1;
                            gelenUcak[j].gecikme_suresi++;
                            if(gelenUcak[j].inis_saati<=24 && gelenUcak[j].kalkis_saati<=24)
                            {
                                printf("%d %d %d degerli ucak talep edilen saatinin dolu olmasi sebebi %d saatine ertelenmistir.\n",gelenUcak[j].oncelik_id,gelenUcak[j].ucak_id,gelenUcak[j].talep_edilen_inis_saati,gelenUcak[j].inis_saati);
                                inis_uygun_mu();
                            }
                            else
                            {
                                printf("%d %d %d degerli ucak gun icinde inis yapamadigi icin iptal edilmistir.\n",gelenUcak[j].oncelik_id,gelenUcak[j].ucak_id,gelenUcak[j].talep_edilen_inis_saati);
                                int sil=dequeue(gelenUcak,rear,gelenUcak[j],j);
                                rear=sil;
                            }
                        }
                        else
                        {
                            // i iptal
                            if(gelenUcak[i].inis_saati<=24 && gelenUcak[i].kalkis_saati<=24)
                            {
                                printf("%d %d %d degerli ucak 3 kezden fazla ertelenmek zorunda kaldigi icin baska havaalanina yonlendirilmistir.\n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati);
                                int sil=dequeue(gelenUcak,rear,gelenUcak[i],i);
                                rear=sil;
                            }
                            else
                            {
                                printf("%d %d %d degerli ucak gun icinde inis yapamadigi icin iptal edilmistir.\n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati);

                                int sil=dequeue(gelenUcak,rear,gelenUcak[i],i);
                                rear=sil;
                            }
                        }
                    }
                    else if(gelenUcak[i].oncelik_id < gelenUcak[j].oncelik_id)
                    {
                        if(gelenUcak[j].gecikme_suresi<3 && gelenUcak[i].gecikme_suresi<3)
                        {
                        	// j ertelenecek
                            gelenUcak[j].inis_saati += 1;
                            gelenUcak[j].kalkis_saati += 1;
                            gelenUcak[j].gecikme_suresi++;
                            if(gelenUcak[j].inis_saati<=24 && gelenUcak[j].kalkis_saati<=24)
                            {
                                printf("%d %d %d degerli ucak talep edilen saatinin dolu olmasi sebebi %d saatine ertelenmistir.\n",gelenUcak[j].oncelik_id,gelenUcak[j].ucak_id,gelenUcak[j].talep_edilen_inis_saati,gelenUcak[j].inis_saati);
                                inis_uygun_mu();
                            }
                            else
                            {
                                printf("%d %d %d degerli ucak gun icinde inis yapamadigi icin iptal edilmistir.\n",gelenUcak[j].oncelik_id,gelenUcak[j].ucak_id,gelenUcak[j].talep_edilen_inis_saati);
                                int sil=dequeue(gelenUcak,rear,gelenUcak[j],j);
                                rear=sil;
                            }
                        }
                        else if(gelenUcak[j].gecikme_suresi<3 && gelenUcak[i].gecikme_suresi==3)
                        {
                        	// j ertelenecek
                            gelenUcak[j].inis_saati += 1;
                            gelenUcak[j].kalkis_saati += 1;
                            gelenUcak[j].gecikme_suresi++;
                            if(gelenUcak[j].inis_saati<=24 && gelenUcak[j].kalkis_saati<=24)
                            {
                                printf("%d %d %d degerli ucak talep edilen saatinin dolu olmasi sebebi %d saatine ertelenmistir.\n",gelenUcak[j].oncelik_id,gelenUcak[j].ucak_id,gelenUcak[j].talep_edilen_inis_saati,gelenUcak[j].inis_saati);
                                inis_uygun_mu();
                            }
                            else
                            {
                                printf("%d %d %d degerli ucak gun icinde inis yapamadigi icin iptal edilmistir.\n",gelenUcak[j].oncelik_id,gelenUcak[j].ucak_id,gelenUcak[j].talep_edilen_inis_saati);
                                int sil=dequeue(gelenUcak,rear,gelenUcak[j],j);
                                rear=sil;
                            }
                        }
                        else if(gelenUcak[j].gecikme_suresi==3 && gelenUcak[i].gecikme_suresi<3)
                        {
                            // j ertelenemiyor i ertelenecek
                            printf("%d %d %d degerli ucak 3 kez ertelendigi icin %d saatinde oncelikli olarak indirilmistir.\n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati,gelenUcak[i].inis_saati);
                            gelenUcak[i].inis_saati += 1;
                            gelenUcak[i].kalkis_saati += 1;
                            gelenUcak[i].gecikme_suresi++;
                            if(gelenUcak[i].inis_saati<=24 && gelenUcak[i].kalkis_saati<=24)
                            {
                                printf("%d %d %d degerli ucak talep edilen saatinin dolu olmasi sebebi %d saatine ertelenmistir.\n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati,gelenUcak[i].inis_saati);
                                inis_uygun_mu();
                            }
                            else
                            {
                                printf("%d %d %d degerli ucak gun icinde inis yapamadigi icin iptal edilmistir.\n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati);
                                int sil=dequeue(gelenUcak,rear,gelenUcak[i],i);
                                rear=sil;
                            }
                        }
                        else
                        {
                            // j iptal
                            if(gelenUcak[j].inis_saati<=24 && gelenUcak[j].kalkis_saati<=24)
                            {
                                printf("%d %d %d degerli ucak 3 kezden fazla ertelenmek zorunda kaldigi icin baska havaalanina yonlendirilmistir.\n",gelenUcak[j].oncelik_id,gelenUcak[j].ucak_id,gelenUcak[j].talep_edilen_inis_saati);
                                int sil=dequeue(gelenUcak,rear,gelenUcak[j],j);
                                rear=sil;
                            }
                            else
                            {
                                printf("%d %d %d degerli ucak gun icinde inis yapamadigi icin iptal edilmistir.\n",gelenUcak[j].oncelik_id,gelenUcak[j].ucak_id,gelenUcak[j].talep_edilen_inis_saati);
                                int sil=dequeue(gelenUcak,rear,gelenUcak[j],j);
                                rear=sil;
                            }
                        }
                    }
                    else
                    {
                        if(gelenUcak[i].ucak_id > gelenUcak[j].ucak_id)
                        {
                            if(gelenUcak[i].gecikme_suresi<3 && gelenUcak[j].gecikme_suresi<3)
                            {
                                // i ertelenecek
                                gelenUcak[i].inis_saati += 1;
                                gelenUcak[i].kalkis_saati += 1;
                                gelenUcak[i].gecikme_suresi++;
                                if(gelenUcak[i].inis_saati<=24 && gelenUcak[i].kalkis_saati<=24)
                                {
                                    printf("%d %d %d degerli ucak talep edilen saatinin dolu olmasi sebebi %d saatine ertelenmistir.\n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati,gelenUcak[i].inis_saati);
                                    inis_uygun_mu();
                                }
                                else
                                {
                                    printf("%d %d %d degerli ucak gun icinde inis yapamadigi icin iptal edilmistir.\n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati);
                                    int sil=dequeue(gelenUcak,rear,gelenUcak[i],i);
                                    rear=sil;
                                }
                            }
                            else if(gelenUcak[i].gecikme_suresi<3 && gelenUcak[j].gecikme_suresi==3)
                            {
                                // i ertelenecek
                                gelenUcak[i].inis_saati += 1;
                                gelenUcak[i].kalkis_saati += 1;
                                gelenUcak[i].gecikme_suresi++;
                                if(gelenUcak[i].inis_saati<=24 && gelenUcak[i].kalkis_saati<=24)
                                {
                                    printf("%d %d %d degerli ucak talep edilen saatinin dolu olmasi sebebi %d saatine ertelenmistir.\n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati,gelenUcak[i].inis_saati);
                                    inis_uygun_mu();
                                }
                                else
                                {
                                    printf("%d %d %d degerli ucak gun icinde inis yapamadigi icin iptal edilmistir.\n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati);
                                    int sil=dequeue(gelenUcak,rear,gelenUcak[i],i);
                                    rear=sil;
                                }
                            }
                            else if(gelenUcak[i].gecikme_suresi==3 && gelenUcak[j].gecikme_suresi<3)
                            {
                                // i ertelenemiyor j ertelenecek
                                printf("%d %d %d degerli ucak 3 kez ertelendigi icin %d saatinde oncelikli olarak indirilmistir.\n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati,gelenUcak[i].inis_saati);
                                gelenUcak[j].inis_saati += 1;
                                gelenUcak[j].kalkis_saati += 1;
                                gelenUcak[j].gecikme_suresi++;
                                if(gelenUcak[j].inis_saati<=24 && gelenUcak[j].kalkis_saati<=24)
                                {
                                    printf("%d %d %d degerli ucak talep edilen saatinin dolu olmasi sebebi %d saatine ertelenmistir.\n",gelenUcak[j].oncelik_id,gelenUcak[j].ucak_id,gelenUcak[j].talep_edilen_inis_saati,gelenUcak[j].inis_saati);
                                    inis_uygun_mu();
                                }
                                else
                                {
                                    printf("%d %d %d degerli ucak gun icinde inis yapamadigi icin iptal edilmistir.\n",gelenUcak[j].oncelik_id,gelenUcak[j].ucak_id,gelenUcak[j].talep_edilen_inis_saati);
                                    int sil=dequeue(gelenUcak,rear,gelenUcak[j],j);
                                    rear=sil;
                                }
                            }
                            else
                            {
                                // i iptal
                                if(gelenUcak[i].inis_saati<=24 && gelenUcak[i].kalkis_saati<=24)
                                {
                                    printf("%d %d %d degerli ucak 3 kezden fazla ertelenmek zorunda kaldigi icin baska havaalanina yonlendirilmistir.\n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati);
                                    int sil=dequeue(gelenUcak,rear,gelenUcak[i],i);
                                    rear=sil;
                                }
                                else
                                {
                                    printf("%d %d %d degerli ucak gun icinde inis yapamadigi icin iptal edilmistir.\n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati);
                                    int sil=dequeue(gelenUcak,rear,gelenUcak[i],i);
                                    rear=sil;
                                }
                            }
                        }
                        else if(gelenUcak[i].ucak_id < gelenUcak[j].ucak_id)
                        {

	                        if(gelenUcak[j].gecikme_suresi<3 && gelenUcak[i].gecikme_suresi<3)
	                        {
	                        	// j ertelenecek
	                            gelenUcak[j].inis_saati += 1;
	                            gelenUcak[j].kalkis_saati += 1;
	                            gelenUcak[j].gecikme_suresi++;
	                            if(gelenUcak[j].inis_saati<=24 && gelenUcak[j].kalkis_saati<=24)
	                            {
                                    printf("%d %d %d degerli ucak talep edilen saatinin dolu olmasi sebebi %d saatine ertelenmistir.\n",gelenUcak[j].oncelik_id,gelenUcak[j].ucak_id,gelenUcak[j].talep_edilen_inis_saati,gelenUcak[j].inis_saati);
                                    inis_uygun_mu();
                                }
                                else
                                {
                                    printf("%d %d %d degerli ucak gun icinde inis yapamadigi icin iptal edilmistir.\n",gelenUcak[j].oncelik_id,gelenUcak[j].ucak_id,gelenUcak[j].talep_edilen_inis_saati);
                                    int sil=dequeue(gelenUcak,rear,gelenUcak[j],j);
                                    rear=sil;
                                }
	                        }
	                        else if(gelenUcak[j].gecikme_suresi<3 && gelenUcak[i].gecikme_suresi==3)
	                        {
	                        	// j ertelenecek
	                            gelenUcak[j].inis_saati += 1;
	                            gelenUcak[j].kalkis_saati += 1;
	                            gelenUcak[j].gecikme_suresi++;
	                            if(gelenUcak[j].inis_saati<=24 && gelenUcak[j].kalkis_saati<=24)
	                            {
                                    printf("%d %d %d degerli ucak talep edilen saatinin dolu olmasi sebebi %d saatine ertelenmistir.\n",gelenUcak[j].oncelik_id,gelenUcak[j].ucak_id,gelenUcak[j].talep_edilen_inis_saati,gelenUcak[j].inis_saati);
                                    inis_uygun_mu();
                                }
                                else
                                {
                                    printf("%d %d %d degerli ucak gun icinde inis yapamadigi icin iptal edilmistir.\n",gelenUcak[j].oncelik_id,gelenUcak[j].ucak_id,gelenUcak[j].talep_edilen_inis_saati);
                                    int sil=dequeue(gelenUcak,rear,gelenUcak[j],j);
                                    rear=sil;
                                }
	                        }
	                        else if(gelenUcak[j].gecikme_suresi==3 && gelenUcak[i].gecikme_suresi<3)
	                        {
	                            // j ertelenemiyor i ertelenecek
	                            gelenUcak[i].inis_saati += 1;
	                            gelenUcak[i].kalkis_saati += 1;
	                            gelenUcak[i].gecikme_suresi++;
	                            if(gelenUcak[i].inis_saati<=24 && gelenUcak[i].kalkis_saati<=24)
	                            {
                                    printf("%d %d %d degerli ucak talep edilen saatinin dolu olmasi sebebi %d saatine ertelenmistir.\n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati,gelenUcak[i].inis_saati);
                                    inis_uygun_mu();
                                }
                                else
                                {
                                    printf("%d %d %d degerli ucak gun icinde inis yapamadigi icin iptal edilmistir.\n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati);
                                    int sil=dequeue(gelenUcak,rear,gelenUcak[i],i);
                                    rear=sil;
                                }
	                        }
	                        else
	                        {
	                            // j iptal
	                            if(gelenUcak[j].inis_saati<=24 && gelenUcak[j].kalkis_saati<=24)
                                {
                                    printf("%d %d %d degerli ucak 3 kezden fazla ertelenmek zorunda kaldigi icin baska havaalanina yonlendirilmistir.\n",gelenUcak[j].oncelik_id,gelenUcak[j].ucak_id,gelenUcak[j].talep_edilen_inis_saati);
                                    int sil=dequeue(gelenUcak,rear,gelenUcak[j],j);
                                    rear=sil;
                                }
                                else
                                {
                                    printf("%d %d %d degerli ucak gun icinde inis yapamadigi icin iptal edilmistir.\n",gelenUcak[j].oncelik_id,gelenUcak[j].ucak_id,gelenUcak[j].talep_edilen_inis_saati);
                                    int sil=dequeue(gelenUcak,rear,gelenUcak[j],j);
                                    rear=sil;
                                }
	                        }
                        }
                    }
                }
            }
        }
	}

	if(rear>=24)
    {
        for(x=rear-1;x>=front;x--)
        {
        	for(y=front;y<rear-1;y++)
            {
                if(gelenUcak[x].inis_saati==gelenUcak[y].inis_saati)
                {
                    if(gelenUcak[x].oncelik_id>gelenUcak[y].oncelik_id)
                    {
                        printf("%d %d %d degerli ucak, daha oncelikli %d %d %d ucagi sebebi ile baska havaalanina yonlendirildi.\n",gelenUcak[x].oncelik_id,gelenUcak[x].ucak_id,gelenUcak[x].talep_edilen_inis_saati,gelenUcak[y].oncelik_id,gelenUcak[y].ucak_id,gelenUcak[y].talep_edilen_inis_saati);
                        int sil=dequeue(gelenUcak,rear,gelenUcak[x],x);
                        rear=sil;
                    }
                    else if(gelenUcak[x].oncelik_id<gelenUcak[y].oncelik_id)
                    {
                        printf("%d %d %d degerli ucak, daha oncelikli %d %d %d ucagi sebebi ile baska havaalanina yonlendirildi.\n",gelenUcak[y].oncelik_id,gelenUcak[y].ucak_id,gelenUcak[y].talep_edilen_inis_saati,gelenUcak[x].oncelik_id,gelenUcak[x].ucak_id,gelenUcak[x].talep_edilen_inis_saati);
                        int sil=dequeue(gelenUcak,rear,gelenUcak[y],y);
                        rear=sil;
                    }
                }
            }
        }
    }
}

void oncelikli_yap()
{
    int i,j;
    struct siralama eleman;
    for(i=front+1;i<rear;i++)
    {
        eleman=gelenUcak[i];
        for(j=i-1;j>=front && gelenUcak[j].inis_saati>eleman.inis_saati;j--)
        {
            gelenUcak[j+1]=gelenUcak[j];
        }
        gelenUcak[j+1]=eleman;
    }
}

void output_guncelle()
{
    FILE *output;
    output=fopen("output.txt","w");
    int i;
    fprintf(output,"oncelik_id,ucak_id,talep_edilen_inis_saati,inis_saati,gecikme_suresi,kalkis_saati\n");
    for(i=front;i<rear;i++)
        fprintf(output,"%d\t %d\t %d\t %d\t %d\t %d\t \n",gelenUcak[i].oncelik_id,gelenUcak[i].ucak_id,gelenUcak[i].talep_edilen_inis_saati,gelenUcak[i].inis_saati,gelenUcak[i].gecikme_suresi,gelenUcak[i].kalkis_saati);
    printf("\n");
    fclose(output);
}
