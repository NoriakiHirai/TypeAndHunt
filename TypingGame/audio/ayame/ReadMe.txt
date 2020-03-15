_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/
【タイトル】 サウンドドライバー Ayame
【ファイル】 Ayame.zip
【作成月日】 2003/07/25 - 2014/08/30
【カテゴリ】 サウンドドライバー
【制 作 者】 さざんた（原作：葉迩倭）
【動作環境】 Windows2000以降
【配布形態】 フリーウェア
【 E-Mail 】 sazanta@wb3.so-net.ne.jp
【HomePage】 http://ayame-lib.blog.so-net.ne.jp
【転　　載】 自由
_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/_/

●説明●
　Ayameは
　　無圧縮PCM Waveファイル
　　OggVorbisファイル
　　圧縮Waveファイル
　　MP3ファイル
　の再生を行う為のWindows用サウンドドライバーみたいなものです。
　MIDIファイルは再生できません。

　AyameS.dll　　－　無圧縮PCM Waveファイルのみ再生できる最小構成版です。

　Ayame.dll　　 －　一般的なDLLです。
　　　　　　　　　　対応している全ての形式を再生可能です。

　AyameSSE.dll　－　SSE2対応版DLLです。
　　　　　　　　　　対応している全ての形式を再生可能です。

　AyameCLR.dll　－　Ayame.dllを.NET2.0以降から使用するためのラッパーDLLです。

  AyameにはShiftJIS版とUnicode版の2種類があります。


●使用法●
　サンプルを参考にしてください。
　AyameTest1.exe
　　WindowsAPIだけを使用した再生アプリケーション

　AyameTest2.exe
　　Windows .NET2.0でC#を使用した再生アプリケーション


●機能●
  DWORD Ayame_GetVersion( void )
　　DLLバージョンの取得
      バージョンを16進数で表した場合、0xAAAABBCCとなり、
      AAAAはメジャーバージョン
      BBはマイナーバージョン
      CCはリビジョンを表します。
      メジャーバージョン・マイナーバージョンが異なる場合、互換性がない場合があります。

  void Ayame_DirectSoundEnumerate( LPDSENUMCALLBACK callback, LPVOID  lpContext )
　　サウンドデバイスの列挙
　　　使用できるサウンドデバイスを列挙します。
　　　ここで取得できたデバイスのGUIDをAyame_Initialize時に指定することにより、
　　　再生デバイスを選択することができます。

　bool Ayame_Initialize( HWND hWnd, void **ppDirectSound, LPCGUID lpcGuid = NULL )
　　ドライバー初期化
　　　使用される引数はどちら方片方であり、
　　　hWndはppDirectSoundがNULLの時であり、
　　　内部でDirectSoundを初期化する必要があるとき。
　　　外部でDirectSoundをすでに生成済みの場合は
　　　ppDrectSoundに渡すことでそれが使われる。

　void Ayame_Uninitialize( void )
　　ドライバー終了。
　　　内部で使用しているメモリの解放などを行う。
　　　生成、あるいは取得したDirectSoundオブジェクトは
　　　この関数内で解放される。

　bool Ayame_GetDirectSoundCaps( DSCAPS *Caps )
　　DirectSoundのCapsの取得

　CAyame *Ayame_CreateInstanceFromFile( const TCHAR *pFileName, unsigned long Start, unsigned long Size, unsigned long Flag )
　　ドライバーのインターフェースであるCAyameクラスを取得するための関数。
　　ローカルドライブにあるサウンドファイルを再生するための関数です。

　　pFileName
　　　読み込みファイル名

　　Start
　　　ファイル内開始Byte

　　Size
　　　ファイル内サイズByte

　　Flag
　　　AYAME_LOADFLAG_GLOBALを指定することでウィンドウが非アクティブ時でも再生されます。
　　　AYAME_LOADFLAG_STREAMを指定することでストリーム方式による再生を行います。
　　　AYAME_LOADFLAG_STATICを指定することで一括再生方式による再生を行います。

　CAyame *Ayame_CreateInstanceFromMemory( void *pData, unsigned long Size, unsigned long Flag )
　　ドライバーのインターフェースであるCAyameクラスを取得するための関数。
　　メモリ上にあるサウンドデータを再生するための関数です。

　　pData
　　　読み込みデータ

　　Size
　　　読み込みデータサイズ

　　Flag
　　　AYAME_LOADFLAG_GLOBALを指定することでウィンドウが非アクティブ時でも再生されます。
　　　AYAME_LOADFLAG_STREAMを指定することでストリーム方式による再生を行います。
　　　AYAME_LOADFLAG_STATICを指定することで一括再生方式による再生を行います。

　CAyame *Ayame_CreateInstanceFromUrl( const TCHAR *pUrl, unsigned long Start, unsigned long Size, unsigned long Flag )
　　ドライバーのインターフェースであるCAyameクラスを取得するための関数。
　　インターネット上にあるサウンドファイルを再生するための関数です。

　　pUrl
　　　読み込みURL名

　　Start
　　　ファイル内開始Byte

　　Size
　　　読み込みデータサイズ

　　Flag
　　　AYAME_LOADFLAG_GLOBALを指定することでウィンドウが非アクティブ時でも再生されます。
　　　AYAME_LOADFLAG_STREAMを指定することでストリーム方式による再生を行います。
　　　AYAME_LOADFLAG_STATICを指定することで一括再生方式による再生を行います。

　bool Ayame_GetLastError( TCHAR *pErrStr, unsigned long Size )
　　最後に発生したエラーを取得する

　　pErrStr
　　　エラー文字列格納先

　　Size
　　　格納バッファサイズ


●諸注意●
　このプログラムを用いることによって何らかの被害、損失が貴方にあったとしても、
　当方は一切責任を負いませんのでご了承ください。
　圧縮WavファイルおよびMP3ファイルのデコードは、WindowsのACM(Audio Compression Manager)を使用しています。
　このためデコーダーが予めインストールされていない環境では、デコードが行えず再生できません。


●再配布●
　再配布などは自由に行っていただいてかまいません。
　また再配布を行う際に私の名前をドキュメントなどに明記する必要はありません。
　事前事後にかかわらず私への連絡なども一切必要ありません。


●謝辞●
　圧縮Wave周りの処理は、AKIYAMA Kouheiさんの情報を参考にさせていただきました。


●ライセンス●
　Ogg, Vorbis, Xiph.org Foundation and their logos are trademarks (tm) of the Xiph.org Foundation.
　Vox uses their SDK, especially ogg_static.lib vorbis_static.lib vorbisfile_static.lib
　These library are copyright (C) 1994-2001 Xiph.org. All rights reserved.


●更新履歴●
　2014.08.30 : phase 6.0.0
　　LoadDLL()関数がInitialize()関数を呼ばないよう修正。
　　　これにより明示的にInitialize()関数を呼ぶ必要があります。
　　SoundDeviceEnumerate()関数を追加。
　　Uninitialize()関数を追加。
　　5.x.xとの互換性はありません。

　2008.10.27 : phase 5.3.0
　　GetVersion()関数を追加。
　　GetDirectSoundCaps()関数を追加。
　　5.2.0との互換性はありません。

　2008.10.08 : phase 5.2.0
　　従来のストリーミング再生に加え、一括再生に対応。
　　一部APIの引数を変更したため5.10との互換性はありません。
　　Play()に再生開始位置を指定できるよう機能追加。

　2008.10.03 : phase 5.1.0
　　unicodeに対応。

　2006.10.10 : phase 5.05
　　SSE対応版のDLLも作成。
　　SSE対応版はSSE2の機能を使っているのでPentium4/Athlon64以降のCPUでないと動作しません。

　2006.08.21 : phase 5.04
　　COMの初期化をしていないのに解放処理を行う場合があった不具合の修正。

　2006.07.24 : phase 5.03
　　ogg/vorbisのライブラリを最新版にしてリビルド。

　2005.08.23 : phase 5.02
　　再生中のファイルのファイルポインタを外部で変更された場合に、
　　再生が正しく行えなくなる不具合を修正。

　2005.08.08 : phase 5.01
　　8BitWave時にノイズが入る不具合を修正。

　2005.08.05 : phase 5.00
　　Subversionによりオープンソース化。
　　圧縮Waveに対応。
　　内部処理を大幅に変更。
　　再生時にノイズが入る問題を解消。

　2005.07.28 : phase 4.06
　　サウンドバッファの更新を1/4秒単位に行うようにしました。
　　停止時にサウンドバッファをクリアするようにしました。

　2005.06.29 : phase 4.04
　　サウンドバッファの更新を1秒から1/2秒に変更。
　　イベントの数を16から32に拡張。

　2005.06.28 : phase 4.03
　　Ayame_CreateInstanceFromHandle()関数で
　　読み込んだファイルが正しく再生されない不具合を修正。

　2005.06.27 : phase 4.02
　　初回ループ再生時に２秒以下のサウンドの時、再生後若干のウェイトが入る不具合の修正。
　　非アクティブウィンドウ時再生の処理の不具合を修正。

　2005.06.26 : phase 4.01
　　Wave再生時に最後にノイズが入る不具合の修正。
　　Wave再生時にループ処理が正しく行われない不具合の修正。

　2005.06.25 : phase 4.00
　　BCC／GCCで落ちる不具合を修正。
　　OggVorbisだけでなく未圧縮Waveの再生にも対応。
　　ロード時のブロックを緩和。
　　オンメモリ再生機能を削除。
　　非アクティブ時の再生に対応。

　2005.06.13 : phase 3.70
　　コンパイラの違いを吸収するためにファイル操作関係をコールバック関数形式に変更。
　　ファイルのサイズ処理の部分の不具合を修正。

　2005.05.16 : phase 3.50
　　2秒以下のサウンドを自動的に非ストリーミング再生を行うように変更。

　2004.11.27 : phase 3.40
　　非ストリーミング再生処理に対応。
　　SEやボイスなどの短い音声の場合はストリーミング
　　させないほうが効率がいい場合が多いです。

　2004.10.27 : phase 3.10
　　ファイルハンドルと開始アドレス、サイズを引数にして
　　インスタンスを生成する関数を追加。
　　コレによりメモリを圧迫せずにストリーミング再生が可能になります。

　2004.09.21 : phase 3.01
　　再生中かどうを取得するためのIsPlay()関数を追加。

　2004.06.24 : phase 3.00
　　内部処理を一新。再生はストリーミングのみに戻し、
　　短すぎて再生できないファイルをはじくように変更。

　2004.06.21 : phase 2.00
　　構造変更に伴い、デコードデータを全てメモリ内に展開するように変更。
　　後に、ストリーミング再生との選択式に出来るように修正していきます。
　　また現状のバージョンではメモリからの読み込みも出来なくなっています。

　2004.02.05 : phase 1.50
　　メモリからの読み込み機能をサポート。
　　それに伴い、ファイルからの読み込みも、
　　一度メモリ内に展開してから再生するように
　　変更しました。
　　またメモリ内に展開されるのは元のファイルであり
　　デコード後のデータがメモリ内に
　　展開されるわけではありません。

　2003.10.07 : phase 1.40
　　DirectSound8を使う意味がないので、
　　DirectSoundでの再生に変更。
　　これでDirectXのバージョンを気にしなくても
　　よくなったと思います。

　2003.10.05 : phase 1.30
　　特定のタイミングでバッファ不足が発生し
　　メモリエラーで落ちるバグを修正。

　2003.08.08 : phase 1.20
　　１秒ごとにおかしくなる場合があるのを修正。

　2003.07.28 : phase 1.10
　　スレッド関連の処理をまもとにした。

　2003.07.25 : phase 1.00
　　とりあえず公開ってことで。
