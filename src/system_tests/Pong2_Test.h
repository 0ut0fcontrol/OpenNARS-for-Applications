/* 
 * The MIT License
 *
 * Copyright 2020 The OpenNARS authors.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

bool YAN_Pong_Left_executed = false;
void YAN_Pong_Left()
{
    YAN_Pong_Left_executed = true;
}
bool YAN_Pong_Right_executed = false;
void YAN_Pong_Right()
{
    YAN_Pong_Right_executed = true;
}
bool YAN_Pong_Stop_executed = false;
void YAN_Pong_Stop()
{
    YAN_Pong_Stop_executed = true;
}
void YAN_Pong2(long iterations)
{
    YAN_INIT();
    puts(">>YAN Pong start");
    YAN_AddOperation(Narsese_AtomicTerm("^left"), YAN_Pong_Left); 
    YAN_AddOperation(Narsese_AtomicTerm("^right"), YAN_Pong_Right); 
    YAN_AddOperation(Narsese_AtomicTerm("^stop"), YAN_Pong_Stop); 
    int szX = 50;
    int szY = 20;
    int ballX = szX/2;
    int ballY = szY/5;
    int batX = 20;
    int batVX = 0;
    int batWidth = 6; //"radius", batWidth from middle to the left and right
    int vX = 1;
    int vY = 1;
    int hits = 0;
    int misses = 0;
    int t=0;
    while(1)
    {
        t++;
        if(iterations != -1 && t++ > iterations)
        {
            exit(0);
        }
        //if(t%10000 == 0)
        //    getchar();
        fputs("\033[1;1H\033[2J", stdout); //POSIX clear screen
        for(int i=0; i<batX-batWidth+1; i++)
        {
            fputs(" ", stdout);
        }
        for(int i=0; i<batWidth*2-1+MIN(0,batX) ;i++)
        {
            fputs("@", stdout);
        }
        puts("");
        for(int i=0; i<ballY; i++)
        {
            for(int k=0; k<szX; k++)
            {
                fputs(" ", stdout);
            }
            puts("|");
        }
        for(int i=0; i<ballX; i++)
        {
            fputs(" ", stdout);
        }
        fputs("#", stdout);
        for(int i=ballX+1; i<szX; i++)
        {
            fputs(" ", stdout);
        }
        puts("|");
        for(int i=ballY+1; i<szY; i++)
        {
            for(int k=0; k<szX; k++)
            {
                fputs(" ", stdout);
            }
            puts("|");
        }
        if(batX <= ballX - batWidth)
        {
            YAN_AddInputBelief(Narsese_AtomicTerm("ball_right"));
        }
        else
        if(ballX + batWidth < batX)
        {
            YAN_AddInputBelief(Narsese_AtomicTerm("ball_left"));
        }
        else
        {
            YAN_AddInputBelief(Narsese_AtomicTerm("ball_equal"));
        }
        YAN_AddInputGoal(Narsese_AtomicTerm("good_yan"));
        if(ballX <= 0)
        {
            vX = 1;
        }
        if(ballX >= szX-1)
        {
            vX = -1;
        }
        if(ballY <= 0)
        {
            vY = 1;
        }
        if(ballY >= szY-1)
        {
            vY = -1;
        }
        if(t%2 == -1)
        {
            ballX += vX;
        }
        ballY += vY;
        if(ballY == 0)
        {
            if(abs(ballX-batX) <= batWidth)
            {
                YAN_AddInputBelief(Narsese_AtomicTerm("good_yan"));
                puts("good");
                hits++;
            }
            else
            {
                puts("bad");
                misses++;
            }
        }
        if(ballY == 0 || ballX == 0 || ballX >= szX-1)
        {
            ballY = szY/2+rand()%(szY/2);
            ballX = rand()%szX;
            vX = rand()%2 == 0 ? 1 : -1;
        }
        if(YAN_Pong_Left_executed)
        {
            YAN_Pong_Left_executed = false;
            puts("Exec: op_left");
            batVX = -3;
        }
        if(YAN_Pong_Right_executed)
        {
            YAN_Pong_Right_executed = false;
            puts("Exec: op_right");
            batVX = 3;
        }
        if(YAN_Pong_Stop_executed)
        {
            YAN_Pong_Stop_executed = false;
            puts("Exec: op_stop");
            batVX = 0;
        }
        batX=MAX(-batWidth*2,MIN(szX-1+batWidth,batX+batVX*batWidth/2));
        printf("Hits=%d misses=%d ratio=%f time=%ld\n", hits, misses, (float) (((float) hits) / ((float) hits + misses)), currentTime);
        if(iterations == -1)
        {
            nanosleep((struct timespec[]){{0, 20000000L}}, NULL); //POSIX sleep
        }
        //YAN_Cycles(10);
    }
}
