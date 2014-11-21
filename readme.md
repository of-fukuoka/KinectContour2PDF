## やりたいこと

- 人＋台座の輪郭をベクターデータで書き出したい。（台座はソフト側で足す）
- 書き出したベクターデータはレーザーカッターに入力する想定

## 仕様

- 人の輪郭はKinectのdepthでとる
- そのままだとギザギザでレーザーが遅いし出来上がりも汚いので、滑らかにする
- 腰に手を当てるとかしたときにできる穴もちゃんと輪郭をとる
- 台座はofRect的にちゃんと直角を出したい（人と一緒に平滑化しちゃダメ）
- ベクターデータはPDFで書き出す（ほんとはSVGにしたいけど面倒そう……）

## 手順

- Kinectからdepth画像を取得
- 上下左右奥行きでクリッピング
- ContourFinderに突っ込んで輪郭抽出（ここで最大/最小面積でフィルタリング）
- 輪郭を平滑化
- 輪郭の内部を塗りつぶして描画
- 台座を同じ色で描画
- 再度ContourFinderに突っ込む（面積のクリッピングは不要）
- 検出された輪郭をPDF書き出し

## 使用addon

- [ofxNI2](https://github.com/satoruhiga/ofxNI2): oFでOpenNIを使うならコレだと思ってます。
- [ofxCv](https://github.com/kylemcdonald/ofxCv): oFでOpenCVを使うならコレだと思ってます。
- ofxOpenCv: ofxCvを使うのに使用。あとofxCvがContourFinderでのHoleの検出が未サポートのようなのでそこでは使ってます。
- [ofxParamEdit](https://github.com/nariakiiwatani/ofxParamEdit): GUIでのリアルタイムパラメータ調整ならコレ。（自分のだけど……あと最近はofxUIも気になってます）
- ofxGui: ofxParamEditを使うのに必要
- [ofxCairo](https://github.com/bgstaal/ofxCairo): 今回初めて使用。PDF書き出しに使ってます。

## ポイント

- ofxNI2はdepth画像のdraw時に使うシェーダーを差し替えられるつくりになってて便利  
上下左右奥行きのクリッピングはこのシェーダーでやってます。（[src/NI.cppの Threshold::getShaderCode() const](https://github.com/of-fukuoka/KinectContour2PDF/blob/master/src/NI.cpp#L130-150) を参照）  
シェーダーなので今回のようにその結果をOpenCVとかで処理したければ一度FBOに書く必要があるけど、ofPixelsをいじるよりは速い気がしてます。

- 輪郭の平滑化はofPolylineで、塗りつぶし描画はofPathでやってる  
これは単純に関数が用意されているかどうかの問題なので別にポイントじゃないけど・・・。

- 台座を足す処理はFBOに書いちゃって改めて輪郭検出してる  
ContourFinderを２度通すことになるので処理コスト的に無駄だけど、書き出し時にしか通んないしまあ良いかなって。。  
パスデータの合成をまともに書くのが嫌だったのが主な理由です。

## ハマりポイント

- ofxCvのバージョン  
ofxCvがmasterブランチで早々と0.9.0対応を進めているのでコンパイル通らず困りました。  
とはいえそこはさすがちゃんとしていて、0.8.4というtagがあったのでそれをcheckoutして解決。

- ofxCvのContourFinderで検出された輪郭がHoleかどうかがわからない  
ofxCvのContourFinderは、現状ではHoleかどうかの区別なく輪郭が検出されます。  
たぶんHoleは頂点の順番が逆まわりなので、それを元に自前で判定する方向もありなんですが書く意欲がわかなかったので輪郭検出にはofxOpenCvのContourFinderを使うことにしました。

- ofxCairoがstrokeを書き出してくれない？  
輪郭だけが欲しかったのでofPath::setFilled(false)とofPath::setStrokeColor(ofColor::black)をしてdrawしたのだけど何も表示してくれない。  
実はofPath::draw()内でstrokeWidth>0をチェックしていて、これでスキップされてたっぽい。(draw()-->tessellate()-->hasOutline()と掘れば見つかります)  
ofPath::setStrokeWidth(1)を追加で設定して解決した。  
描画するのに使うときはstrokeWidth>0でチェックして良いけど、今回みたいにPDF書き出しをするなら太さゼロの線も書き出したいから何か方法があると良いんだけど。  
ofGetCurrentRenderer()->draw(poly)してもだめだった。  
