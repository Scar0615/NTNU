# LineBot（整合版）

#載入各種工具模組與元件
from flask import Flask, request, abort

from linebot import (
    LineBotApi, WebhookHandler
)
from linebot.exceptions import (
    InvalidSignatureError
)
from linebot.models import (
    MessageEvent, TextMessage, TextSendMessage,
)
from linebot.models import (
    BubbleContainer,
    ImageComponent,
    URIAction,
    BoxComponent,
    TextComponent,
    ButtonComponent,
    FlexSendMessage
)

# 建立 Flask 應用
app = Flask(__name__)

# line_bot_api = LineBotApi('YOUR_CHANNEL_ACCESS_TOKEN')
# handler = WebhookHandler('YOUR_CHANNEL_SECRET')

# 設定 Channel (Bot) 的基本認證資訊
secret = '?????'
token = '??????'

line_bot_api = LineBotApi(token)
handler = WebhookHandler(secret)

# 網站路徑
@app.route("/callback", methods=['POST'])
def callback():
    # get X-Line-Signature header value
    signature = request.headers['X-Line-Signature']

    # get request body as text
    body = request.get_data(as_text=True)
    app.logger.info("Request body: " + body)

    # handle webhook body
    try:
        handler.handle(body, signature)
    except InvalidSignatureError:
        print("Invalid signature. Please check your channel access token/channel secret.")
        abort(400)

    return 'OK'

# 加入文字訊息處理程序
@handler.add(MessageEvent, message=TextMessage)
def handle_message(event):
    bot_reply(event)

# 訊息回覆函數定義
def bot_reply(event):
    try:
        msg = event.message.text
        if ('flex' in msg):
            uid = event.source.user_id
            # 建立 Flex 傳送訊息
            bubble = BubbleContainer(
                direction = 'ltr',
                hero = ImageComponent(
                    url='https://etech.gac.ntnu.edu.tw/bot/images/flower.jpg',
                    size='full',
                    aspect_ratio='4:3', # one of 1:1, 2:1, 3:1, 16:9, 4:3, ...
                    aspect_mode='fit', # cover, fit
                    action=URIAction(uri='https://etech.gac.ntnu.edu.tw/bot/images/flower.jpg', label='Flex')
                ),
                body = BoxComponent(
                    layout='vertical',
                    contents=[
                        TextComponent(text='Flex 的標題', weight='bold'),
                        TextComponent(text='Flex 內的文字內容（第一列）'),
                        TextComponent(text='Flex 內的文字內容（第二列）'),
                        TextComponent(text='......（第三列）'),
                        ButtonComponent(
                            style='link',
                            height='sm',
                            action=URIAction(label='按這裡去 Google', uri='http://www.google.com.tw/')
                        ),
                        ButtonComponent(
                            style='link',
                            height='sm',
                            action=URIAction(label='按這裡看地圖', uri='https://www.google.com.tw/maps')
                        )
                    ]
                )
            )
            flx = FlexSendMessage(alt_text='This is my flex message...', contents=bubble)
            line_bot_api.push_message(uid, flx)
        else:
            line_bot_api.reply_message(
                event.reply_token,
                TextSendMessage(text=event.message.text))
    except:
        line_bot_api.reply_message(
            event.reply_token,
            TextSendMessage(text='喔喔、、、有問題了！'))
    return

# 啟動 Flask 服務
if __name__ == "__main__":
    app.run()
