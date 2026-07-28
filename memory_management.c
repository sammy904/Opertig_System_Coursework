#include <stdio.h>

#define MAX 100

void FIFO(int pages[], int n, int frames)
{
    int frame[20];
    int front = 0;
    int faults = 0;
    int hits = 0;

    for(int i=0;i<frames;i++)
        frame[i]=-1;

    printf("\n===== FIFO PAGE REPLACEMENT =====\n\n");

    for(int i=0;i<n;i++)
    {
        int found=0;

        for(int j=0;j<frames;j++)
        {
            if(frame[j]==pages[i])
            {
                found=1;
                hits++;
                break;
            }
        }

        if(!found)
        {
            frame[front]=pages[i];
            front=(front+1)%frames;
            faults++;
        }

        printf("Page %d -> ",pages[i]);

        for(int j=0;j<frames;j++)
        {
            if(frame[j]==-1)
                printf("- ");
            else
                printf("%d ",frame[j]);
        }

        printf("\n");
    }

    printf("\nPage Faults : %d\n",faults);
    printf("Page Hits   : %d\n",hits);

    printf("Hit Ratio   : %.2f\n",(float)hits/n);
    printf("Fault Ratio : %.2f\n",(float)faults/n);
}

void LRU(int pages[],int n,int frames)
{
    int frame[20];
    int time[20];

    int faults=0;
    int hits=0;
    int count=0;

    for(int i=0;i<frames;i++)
    {
        frame[i]=-1;
        time[i]=0;
    }

    printf("\n===== LRU PAGE REPLACEMENT =====\n\n");

    for(int i=0;i<n;i++)
    {
        int found=0;

        for(int j=0;j<frames;j++)
        {
            if(frame[j]==pages[i])
            {
                found=1;
                hits++;
                time[j]=++count;
                break;
            }
        }

        if(!found)
        {
            int pos=0;

            for(int j=1;j<frames;j++)
            {
                if(time[j]<time[pos])
                    pos=j;
            }

            frame[pos]=pages[i];
            time[pos]=++count;

            faults++;
        }

        printf("Page %d -> ",pages[i]);

        for(int j=0;j<frames;j++)
        {
            if(frame[j]==-1)
                printf("- ");
            else
                printf("%d ",frame[j]);
        }

        printf("\n");
    }

    printf("\nPage Faults : %d\n",faults);
    printf("Page Hits   : %d\n",hits);

    printf("Hit Ratio   : %.2f\n",(float)hits/n);
    printf("Fault Ratio : %.2f\n",(float)faults/n);
}

int main()
{
    int n;
    int pages[MAX];
    int frames;

    printf("Number of Pages : ");
    scanf("%d",&n);

    printf("Enter Reference String:\n");

    for(int i=0;i<n;i++)
        scanf("%d",&pages[i]);

    printf("Frames : ");
    scanf("%d",&frames);

    FIFO(pages,n,frames);

    printf("\n");

    LRU(pages,n,frames);

    return 0;
}

