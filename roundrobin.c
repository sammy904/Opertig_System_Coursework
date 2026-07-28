#include<stdio.h>

int main()
{
    int n;
    int bt[20],rt[20];
    int wt[20],tat[20];

    int tq;

    printf("Enter Number of Processes: ");
    scanf("%d",&n);

    for(int i=0;i<n;i++)
    {
        printf("Burst Time P%d: ",i+1);
        scanf("%d",&bt[i]);

        rt[i]=bt[i];
    }

    printf("Time Quantum: ");
    scanf("%d",&tq);

    int time=0;
    int done;

    do
    {
        done=1;

        for(int i=0;i<n;i++)
        {
            if(rt[i]>0)
            {
                done=0;

                if(rt[i]>tq)
                {
                    time+=tq;
                    rt[i]-=tq;
                }
                else
                {
                    time+=rt[i];

                    wt[i]=time-bt[i];

                    rt[i]=0;
                }
            }
        }

    }while(!done);

    printf("\n");

    float avgWT=0;
    float avgTAT=0;

    printf("Process\tBT\tWT\tTAT\n");

    for(int i=0;i<n;i++)
    {
        tat[i]=bt[i]+wt[i];

        avgWT+=wt[i];
        avgTAT+=tat[i];

        printf("P%d\t%d\t%d\t%d\n",
        i+1,
        bt[i],
        wt[i],
        tat[i]);
    }

    printf("\nAverage Waiting Time = %.2f\n",avgWT/n);

    printf("Average Turnaround Time = %.2f\n",avgTAT/n);

    return 0;
}

