#include <iostream>
#include <map>
#include <mirai.h>
#include "EasyFunc.h"

using namespace std;
using namespace Cyan;

string Name[25] = { "Kasumi","Arisa","Rimi","Saya","Tae",
					"Kanon","Hagumi","Kokoro","Michelle","Kaoru",
					"Eve","Hina","Aya","Chisato","Maya",
					"Sayo","Rinko","Lisa","Yukina","Ako",
					"Tomoe","Tsugumi","Himari","Ran","Moca" };
struct Egg {int num[25] = { 0 };};
map<QQ_t, Egg> user;//用户抽了几个蛋，暂时存在缓存里，将来存json
const void ars_egg(MiraiBot& bot)//ars扭蛋功能实现(记得传入bot变量的地址，这里改了好多次)
{
	bot.On<Message>(
		[&](Message m)
		{
			// 只能通过 Message 获得发送者的QQ号码
			// 如果想要“原路回复”(好友消息私聊回复，群组消息群组回复)，
			// 请使用 Message 事件的 Reply 方法。
			//m.Reply("Message事件可处理三种消息:" + m.MessageChain);
			if (m.GetMessageType() == MessageType::GroupMessage)// 判断是否群组消息
			{
				GroupMessage gm = m.ToGroupMessage();
				// TODO: 针对群组消息的特别处理
				try
				{
					string plain = m.MessageChain.GetPlainText();//消息
					if (plain == "ars扭蛋")
					{
						int which = GetRandomInt() % 25;//这里要放一个随机数
						GroupImage img = bot.UploadGroupImage("C:\\Users\\zhou\\Desktop\\bangdream\\" + Num2Str(which + 1) + ".jpg");//图片路径
						user[m.Sender].num[which]++;
						cout << "ars扭蛋 响应" << endl;
						m.Reply(MessageChain().At(m.Sender).Plain("，您扭出的蛋是" + Name[which]).Image(img));
						return;
					}
					if (plain == "ars扭蛋详情")
					{
						string msg = "";
						for (int i = 0; i < 25; ++i)
						{
							if (user[m.Sender].num[i] != 0)
								msg = msg + Name[i] + "数量为" + Num2Str(user[m.Sender].num[i]) + "\n";//暂时存在缓存里，将来存json
						}
						cout << "ars扭蛋详情 响应" << endl;
						m.Reply(MessageChain().At(m.Sender).Plain("您的扭蛋详情为：\n" + msg));
					}
				}
				catch (const std::exception& ex)
				{
					cout << ex.what() << endl;
				}
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