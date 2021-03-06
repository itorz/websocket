
#include "okcoinwebsocketapi.h"


OKCoinWebSocketApiCom *comapi = 0;	//此处为全局变量，建议用户做成单例模式
OKCoinWebSocketApiCn *cnapi = 0;	//此处为全局变量，建议用户做成单例模式

//以下是webstocket回调，由于国际站和国内站分别是两个连接，
//也就是数据是由两个线程发出的，故国际站和国内站有各自的回调函数。
//如果对线程应用不太熟练的开发者，尽量不要用一个回调函数同时接收两个站的数据，那样会使你的程序变得复杂。
void cn_callbak_open()
{
	//向服务器发送命令
	
	std::cout << "国内站连接成功！3秒钟后开始接收数据，输入3断开连接。 " << std::endl;
	Sleep(3000);
	//连接成功后立即接收tick和depth数据
	//另外，把接收行情数据请求放在open回调里作用在于:
	//当意外断开，重新连接后触发本回调可自动发送接收请求。
	//所以尽量要把行情类的接收请求放在本回调里。
	if(cnapi != 0)
	{
		cnapi->ok_btccny_ticker();
		cnapi->ok_btccny_depth();
	}
	
};
void cn_callbak_close()
{
	std::cout << "连接已经断开！ " << std::endl;
};
void cn_callbak_message(const char *message)
{
	std::cout << "Message: " << message << std::endl;
};

//////////////////////////////////

void com_callbak_open()
{
	//向服务器发送命令
	
	std::cout << "国际站连接成功！3秒钟后开始接收数据，输入3断开连接。 " << std::endl;
	Sleep(3000);
	//连接成功后立即接收tick和depth数据
	//另外，把接收行情数据请求放在open回调里作用在于:
	//当意外断开，重新连接后触发本回调可自动发送接收请求。
	//所以尽量要把行情类的接收请求放在本回调里。
	if(comapi != 0)
	{
		comapi->ok_btcusd_ticker();
		comapi->ok_btcusd_depth();
	}
	
};
void com_callbak_close()
{
	std::cout << "连接已经断开！ " << std::endl;
};
void com_callbak_message(const char *message)
{
	std::cout << "Message: " << message << std::endl;
};

int main(int argc, char* argv[]) 
{	
	
	//实例化API
	std::string cn_apiKey		= "";									//请到www.okcoin.cn申请。
	std::string cn_secretKey		= "";								//请到www.okcoin.cn申请。
	cnapi = new OKCoinWebSocketApiCn(cn_apiKey,cn_secretKey);			//国内站
	cnapi->SetCallBackOpen(cn_callbak_open);
	cnapi->SetCallBackClose(cn_callbak_close);
	cnapi->SetCallBackMessage(cn_callbak_message);
	cnapi->Run();//启动连接服务器线程
	

	std::string com_apiKey		= "";			//请到www.okcoin.com申请。
	std::string com_secretKey	= "";			//请到www.okcoin.com申请。
	comapi = new OKCoinWebSocketApiCom(com_apiKey,com_secretKey);				//国际站
	comapi->SetCallBackOpen(com_callbak_open);
	comapi->SetCallBackClose(com_callbak_close);
	comapi->SetCallBackMessage(com_callbak_message);
	comapi->Run();//启动连接服务器线程
	
	Sleep(3000);


	cout << "输入1测试下单功能，输入2注销数据接收请求，输入3关闭连接，请输入：";
	string i;
	cin >> i;
	if(i == "1")
	{
		//期货下单测试
		string symbolcom		=		"ltc_usd";
		string type				=		"buy";//买卖类型： 限价单（buy/sell） 市价单（buy_market/sell_market）
		string price			=		"1";
		string amount			=		"0.1";
		comapi->ok_spotusd_trade(symbolcom,type,price,amount);	//下单交易

		string order_id	= "33732079";
		comapi->ok_spotusd_cancel_order(symbolcom,order_id);	//取消订单
		system("pause");
	}

	//对于已经注册的数据在不需要时注销掉，不再接收，
	//注册数据太多而非必要数据，会使您的程序整全性能下降，请开发者注意处理。
	if(i == "2")
	{
		if(comapi != 0)
		{
			cnapi->remove_ok_btccny_ticker();
			cnapi->remove_ok_btccny_depth();
		}


		if(comapi != 0)
		{
			comapi->remove_ok_btcusd_ticker();
			comapi->remove_ok_btcusd_depth();
		}
		system("pause");
	}

	//关闭连接
	cnapi->Close();	
	comapi->Close();


	system("pause");

	//释放API实例
	delete cnapi;
	delete comapi;
	cout << "释放API实例完成。";

	system("pause");

}

