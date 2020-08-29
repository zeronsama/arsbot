#include <iostream>
#include <map>
#include <mirai.h>
#include "myheader.h"

int GetRandomInt()
{
	srand(time(0));
	return rand();
}
string Num2Str(int64_t num)
{
	char str[16] = { 0 };
	itoa(num, str, 10);
	return str;
}
string Name[25] = { "Kasumi","Arisa","Rimi","Saya","Tae",
					"Kanon","Hagumi","Kokoro","Michelle","Kaoru",
					"Eve","Hina","Aya","Chisato","Maya",
					"Sayo","Rinko","Lisa","Yukina","Ako",
					"Tomoe","Tsugumi","Himari","Ran","Moca" };

int main()
{
	using namespace std;
	using namespace Cyan;

#if defined(WIN32) || defined(_WIN32)
	// 切换代码页，让 CMD 可以显示 UTF-8 字符
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

	struct Egg {
		int num[25] = { 0 };
	};

	map<QQ_t, Egg> user;//用户抽了几个蛋，暂时存在缓存里，将来存json

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
						cout << "ars扭蛋 响应" << endl;
						int which = GetRandomInt() % 25;//这里要放一个随机数
						GroupImage img = bot.UploadGroupImage("C:\\Users\\zhou\\Desktop\\bangdream\\" + Num2Str(which + 1) + ".jpg");//图片路径
						user[m.Sender].num[which]++;
						m.Reply(MessageChain().At(m.Sender).Plain("，您扭出的蛋是" + Name[which]).Image(img));
						return;
					}
					if (plain == "ars扭蛋详情")
					{
						string msg = "";
						for (int i = 0; i < 25; ++i) {
							if (user[m.Sender].num[i] != 0)
								msg = msg + Name[i] + "数量为" + Num2Str(user[m.Sender].num[i]) + "\n";//暂时存在缓存里，将来存json
						}
						m.Reply(MessageChain().At(m.Sender).Plain("您的扭蛋详情为：\n" + msg));
					}
				}
				catch (const std::exception& ex)
				{
					cout << ex.what() << endl;
				}
			}
		});


	bot.EventLoop();

	return 0;
}