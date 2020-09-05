#include <iostream>
#include <map>
#include <mirai.h>
#include "EasyFunc.h"

using namespace std;
using namespace Cyan;

//全局变量初始化
string Name[25] = { "Kasumi","Arisa","Rimi","Saya","Tae",
					"Kanon","Hagumi","Kokoro","Michelle","Kaoru",
					"Eve","Hina","Aya","Chisato","Maya",
					"Sayo","Rinko","Lisa","Yukina","Ako",
					"Tomoe","Tsugumi","Himari","Ran","Moca" };

string SourceAddress = "C:\\Users\\zhou\\source\\repos\\arsbot\\source";//资源路径
//初始化结束

const void ars_egg(MiraiBot& bot)//ars扭蛋功能实现(记得传入bot变量的地址，这里改了好多次),json实现
{
	bot.On<Message>(
		[&](Message m)
		{
			if (m.GetMessageType() == MessageType::GroupMessage)// 判断是否群组消息
			{
				GroupMessage gm = m.ToGroupMessage();//针对群组消息的特别处理
				try
				{
					string plain = m.MessageChain.GetPlainText();//消息
					if (plain == "ars扭蛋")
					{
						int which = GetRandomInt() % 25;//这里要放一个随机数
						nlohmann::json EggData;
						fstream EggDataFile(SourceAddress + "\\EggData.json");//读写json
						EggDataFile >> EggData;
						EggDataFile.seekg(0,ios::beg);//跳至文件头
						if(EggData[Num2Str(m.Sender)].empty()||EggData[Num2Str(m.Sender)][Num2Str(which)].empty())//如果成员为空则创建
						{
							for (int i = 0; i < 25; i++)
								EggData[Num2Str(m.Sender)][Num2Str(i)] = 0;
							EggData[Num2Str(m.Sender)][Num2Str(which)] = 1;
						}
						else//如果成员存在则加1
							EggData[Num2Str(m.Sender)][Num2Str(which)] = EggData[Num2Str(m.Sender)][Num2Str(which)] + 1;
						EggDataFile << EggData;
						EggDataFile.close();
						MiraiImage img = bot.UploadGroupImage(SourceAddress+"\\bangdream\\" + Num2Str(which + 1) + ".jpg");//图片路径
						cout << "ars扭蛋 响应" << endl;
						m.Reply(MessageChain().At(m.Sender).Plain("，您扭出的蛋是" + Name[which]).Image(img));
						return;
					}
					if (plain == "ars扭蛋详情")
					{
						string msg = "";//信息流
						nlohmann::json EggData;
						ifstream EggDataFile(SourceAddress + "\\EggData.json");//读json
						EggDataFile >> EggData;
						EggDataFile.close();
						for (int i = 0; i < 25; i++)
							if (EggData[Num2Str(m.Sender)][Num2Str(i)] != 0)
								msg = msg + Name[i] + "数量为" + Num2Str(EggData[Num2Str(m.Sender)][Num2Str(i)]) + "\n";
						cout << "ars扭蛋详情 响应" << endl;
						m.Reply(MessageChain().At(m.Sender).Plain("您的扭蛋详情为：\n" + msg));
					}
				}
				catch (const std::exception& ex){cout << ex.what() << endl;}
			}
		});
	return;
}

int main()
{
#if defined(WIN32) || defined(_WIN32)
	system("chcp 65001");
#endif

	MiraiBot bot("127.0.0.1", 8080);
	while (true)
	{
		try
		{
			bot.Auth("arsbotstart", 873227452_qq);
			break;
		}
		catch (const std::exception& ex)
		{
			cout << ex.what() << endl;
		}
		MiraiBot::SleepSeconds(1);
	}
	cout << "arsbot is working..." << endl;

	ars_egg(bot);

	bot.EventLoop();//事件轮询

	return 0;
}