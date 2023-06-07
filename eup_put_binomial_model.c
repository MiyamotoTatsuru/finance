#include<stdio.h>
#include<stdbool.h>
#include<string.h>
int main(void){
	/*二項モデルによるヨーロピアンプットオプションの時刻0での価格を求めるコードです.
	各種のパラメーターをキーボードから入力すると時刻0での価格を返します.また各デルタヘッジ価格を求めることもできます.理論的な部分はシュリーヴの「ファイナンスのための確率解析1」の１章をそのままつかっています
	*/
	double u,d,r,s0;
	int N;
	//上昇率をキーボードから入力
	printf("up factor:");
	scanf("%lf",&u);
	//下落率をキーボードから入力
	printf("down factor:");
	scanf("%lf",&d);
	//金利をキーボードから入力
	printf("interest rate:");
	scanf("%lf",&r);
	//裁定機会が生じている場合はエラーを表示する
	if(d<=0) printf("error:d<=0");
	if(d>=1+r) printf("error:d>=1+r,there is an arbitrage\n");
	if(1.0+r>=u) printf("error:1+r>=u,there is an arbitrage\n");
	//リスク中立価格を定義
	double p=(1+r-d)/(u-d);
	double q=1-p;
	//リスク中立価格を表示する
	printf("risk neutral probability of Head:%f\nrisk neutral probability of Tale:%f\n",p,q);
	//モデルの期間の長さをキーボードから入力
	printf("period:");
	scanf("%d",&N);
	//株価の時刻0での値をキーボードから入力
	printf("initial stock price:");
	scanf("%lf",&s0);
	//株価を収納する配列を宣言する.ここでは(i,j)成分にHead(オモテ)がi回,Tale(ウラ)はj回でた場合の株価を記入することにする
	double stock[N+1][N+1];
	//まず配列の各成分に株価の初期値を代入する
	for(int i=0;i<N+1;i++){
		for(int j=0;j<N+1;j++){
			stock[i][j]=s0;
		}
	}
	//つぎに(i,j)成分をu^i*d^j倍する.これで株価の表は完成.
	for(int i=0;i<N+1;i++){
		for(int j=0;j<N+1;j++){
			for(int a=0;a<i;a++) stock[i][j]=(stock[i][j])*u;
			for(int b=0;b<j;b++) stock[i][j]=(stock[i][j])*d;
		}
	}
	/*ここで株価の表の表示を行うコードをコメントアウトさせている.なにか不具合があれば検証に使用する
	for(int i=0;i<N+1;i++){
		for(int j=0;j<N+1;j++) printf("%.2f ",stock[i][j]);
		puts("");
	}*/
	double K;
	//権利行使価格をキーボードから入力する
	printf("strike price:");
	scanf("%lf",&K);
	//このヨーロピアンプットのオプション価格を収納する配列を宣言する.ここでは(i,j)成分にHead(オモテ)がi回,Tale(ウラ)はj回でた場合のオプション価格を記入することにする
	double option_price[N+1][N+1];
	//時刻Nのオプション価格をヨーロピアンプットの定義に従い計算する.つまりmax{行使価格-株価,0}である.
	for(int i=0;i<N+1;i++){
	if(K-stock[N-i][i]<=0){
		option_price[N-i][i]=0;
	} else{
		option_price[N-i][i]=K-stock[N-i][i];
	}
	}
	/*ここでオプション価格を表示するコードをコメントアウトさせている.なにか不具合があれば使用する
	for(int i=0;i<N+1;i++){
		for(int j=0;j<N+1;j++) printf("%.2f ",option_price[i][j]);
		puts("");
	}*/
	//リスク中立評価式に従いオプション価格を順に決定する
	for(int j=1;j<N+1;j++){
	for(int i=0;i<N-j+1;i++){
	option_price[N-j-i][i]=((option_price[N-j-i+1][i])*p+(option_price[N-j-i][i+1])*q)*(1/(1+r));	
		}
	}
	/*オプション価格表を表示するコードをコメントアウトさせている.なにか不具合があれば検証に使用する.
	for(int i=0;i<N+1;i++){
		for(int j=0;j<N+1;j++) printf("%.2f ",option_price[i][j]);
		puts("");
	}*/
	printf("time-zero price of this European-put-option : %.3f\n\n\n",option_price[0][0]);
	printf("if you want to know the value of the delta hedge,enter the number of times H appears,and T appears:\n");
	int h,t;
	scanf("%d%d",&h,&t);
	printf("delta : %f",(option_price[h+1][t]-option_price[h][t+1])/(stock[h+1][t]-stock[h][t+1]));
}
