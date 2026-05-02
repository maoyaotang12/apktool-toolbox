#include "widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QFormLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QStyle>
#include <QCheckBox>

Widget::Widget(QWidget *parent)
: QWidget(parent)
{
    setWindowTitle("APK 全能工具箱");
    setMinimumSize(860,560);

    m_proc = new QProcess(this);
    connect(m_proc, &QProcess::readyReadStandardOutput, this, &Widget::onProcOut);
    connect(m_proc, &QProcess::readyReadStandardError, this, &Widget::onProcOut);
    connect(m_proc, QOverload<int,QProcess::ExitStatus>::of(&QProcess::finished),
            this, &Widget::onProcEnd);

    QVBoxLayout* mainLay = new QVBoxLayout(this);
    mainLay->setContentsMargins(10,10,10,10);
    mainLay->setSpacing(6);

    tabWidget = new QTabWidget;
    tabWidget->addTab(createUnpackPackTab(), "解包/打包");
    tabWidget->addTab(createInfoShellTab(), "信息/查壳");
    tabWidget->addTab(createSignTab(), "签名");
    tabWidget->addTab(createAlignTab(), "对齐");
    tabWidget->addTab(createInstallTab(), "安装");

    mainLay->addWidget(tabWidget);

    m_logEdit = new QTextEdit;
    m_logEdit->setReadOnly(true);
    m_logEdit->setMaximumHeight(180);
    mainLay->addWidget(new QLabel("运行日志："));
    mainLay->addWidget(m_logEdit);

    checkDevice();
}

Widget::~Widget()
{
}

void Widget::logMsg(const QString &txt)
{
    m_logEdit->append(txt);
}

void Widget::onProcOut()
{
    QString out = m_proc->readAllStandardOutput();
    QString err = m_proc->readAllStandardError();
    if(!out.isEmpty()) m_logEdit->append(out);
    if(!err.isEmpty()) m_logEdit->append(err);
}

void Widget::onProcEnd(int code)
{
    logMsg(QString("✅ 进程结束，退出码：%1").arg(code));
}

QPushButton* Widget::makeBtn(const QString& txt, const QString& iconName)
{
    QPushButton* btn = new QPushButton(txt);
    btn->setStyleSheet("");
    if (iconName == "open")
        btn->setIcon(style()->standardIcon(QStyle::SP_DirOpenIcon));
    else if (iconName == "run")
        btn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    else if (iconName == "key")
        btn->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    else if (iconName == "apk")
        btn->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    return btn;
}

// ========================= 解包/打包 =========================
QWidget* Widget::createUnpackPackTab()
{
    QWidget* w = new QWidget;
    QVBoxLayout* lay = new QVBoxLayout(w);

    QGroupBox* grpUnpack = new QGroupBox("APK 解包（自动输出目录）");
    QFormLayout* formUn = new QFormLayout(grpUnpack);

    m_editUnpackApk = new QLineEdit;
    QPushButton* btnSelApk = makeBtn("选择APK", "apk");
    connect(btnSelApk, &QPushButton::clicked, this, &Widget::selectUnpackApk);
    QHBoxLayout* h1 = new QHBoxLayout;
    h1->addWidget(m_editUnpackApk);
    h1->addWidget(btnSelApk);
    formUn->addRow("目标APK：", h1);

    m_editUnpackDir = new QLineEdit;
    formUn->addRow("输出目录：", m_editUnpackDir);

    QPushButton* btnUnpack = makeBtn("开始解包", "run");
    connect(btnUnpack, &QPushButton::clicked, this, &Widget::doUnpack);
    formUn->addRow(btnUnpack);

    lay->addWidget(grpUnpack);

    QGroupBox* grpPack = new QGroupBox("APK 打包（自动输出APK）");
    QFormLayout* formPk = new QFormLayout(grpPack);

    m_editPackDir = new QLineEdit;
    QPushButton* btnSelPkgDir = makeBtn("选择源码目录", "open");
    connect(btnSelPkgDir, &QPushButton::clicked, this, &Widget::selectPackDir);
    QHBoxLayout* h3 = new QHBoxLayout;
    h3->addWidget(m_editPackDir);
    h3->addWidget(btnSelPkgDir);
    formPk->addRow("源码目录：", h3);

    m_editPackOutApk = new QLineEdit;
    formPk->addRow("输出APK：", m_editPackOutApk);

    QPushButton* btnPack = makeBtn("开始打包", "run");
    connect(btnPack, &QPushButton::clicked, this, &Widget::doPack);
    formPk->addRow(btnPack);

    lay->addWidget(grpPack);
    return w;
}

QWidget* Widget::createInfoShellTab()
{
    QWidget* w = new QWidget;
    QVBoxLayout* lay = new QVBoxLayout(w);
    QGroupBox* grp = new QGroupBox("APK 信息查询 + 查壳");
    QFormLayout* form = new QFormLayout(grp);

    m_editShellApk = new QLineEdit;
    QPushButton* btnSel = makeBtn("选择APK", "apk");
    connect(btnSel, &QPushButton::clicked, this, &Widget::selectShellApk);
    QHBoxLayout* h = new QHBoxLayout;
    h->addWidget(m_editShellApk);
    h->addWidget(btnSel);
    form->addRow("APK文件：", h);

    QPushButton* btnCheck = makeBtn("获取信息 + 查壳", "run");
    connect(btnCheck, &QPushButton::clicked, this, &Widget::doCheckShell);
    form->addRow(btnCheck);

    lay->addWidget(grp);
    return w;
}

QWidget* Widget::createSignTab()
{
    QWidget* w = new QWidget;
    QVBoxLayout* lay = new QVBoxLayout(w);
    QGroupBox* grp = new QGroupBox("APK / ZIP 签名（signapk 完整版）");
    QFormLayout* form = new QFormLayout(grp);

    m_editSignApk = new QLineEdit;
    QPushButton* btnSel = makeBtn("选择 APK/ZIP", "apk");
    connect(btnSel, &QPushButton::clicked, this, &Widget::selectSignApk);
    QHBoxLayout* h = new QHBoxLayout;
    h->addWidget(m_editSignApk);
    h->addWidget(btnSel);
    form->addRow("文件 (apk/zip)：", h);

    m_editSignPubKey = new QLineEdit;
    QPushButton* btnSelPub = makeBtn("选择公钥", "key");
    connect(btnSelPub, &QPushButton::clicked, this, &Widget::selectSignPubKey);
    QHBoxLayout* hPub = new QHBoxLayout;
    hPub->addWidget(m_editSignPubKey);
    hPub->addWidget(btnSelPub);
    form->addRow("公钥 x509.pem：", hPub);

    m_editSignPriKey = new QLineEdit;
    QPushButton* btnSelPri = makeBtn("选择私钥", "key");
    connect(btnSelPri, &QPushButton::clicked, this, &Widget::selectSignPriKey);
    QHBoxLayout* hPri = new QHBoxLayout;
    hPri->addWidget(m_editSignPriKey);
    hPri->addWidget(btnSelPri);
    form->addRow("私钥 pk8：", hPri);

    m_cbSignWhole = new QCheckBox("-w 签名整个 ZIP（刷机包）");
    m_cbDisableV2 = new QCheckBox("禁用 V2 签名");
    m_cbEnableV4 = new QCheckBox("启用 V4 签名");

    form->addRow(m_cbSignWhole);
    form->addRow(m_cbDisableV2);
    form->addRow(m_cbEnableV4);

    m_editMinSdk = new QLineEdit("24");
    form->addRow("最小 SDK 版本：", m_editMinSdk);

    m_editAlign = new QLineEdit("4");
    form->addRow("对齐字节：", m_editAlign);

    QPushButton* btnSign = makeBtn("执行签名", "run");
    connect(btnSign, &QPushButton::clicked, this, &Widget::doSign);
    form->addRow(btnSign);

    lay->addWidget(grp);
    return w;
}

QWidget* Widget::createAlignTab()
{
    QWidget* w = new QWidget;
    QVBoxLayout* lay = new QVBoxLayout(w);
    QGroupBox* grp = new QGroupBox("APK 对齐优化");
    QFormLayout* form = new QFormLayout(grp);

    m_editAlignApk = new QLineEdit;
    QPushButton* btnSel = makeBtn("选择APK", "apk");
    connect(btnSel, &QPushButton::clicked, this, &Widget::selectAlignApk);
    QHBoxLayout* h = new QHBoxLayout;
    h->addWidget(m_editAlignApk);
    h->addWidget(btnSel);
    form->addRow("APK文件：", h);

    QPushButton* btnAlign = makeBtn("执行对齐", "run");
    connect(btnAlign, &QPushButton::clicked, this, &Widget::doAlign);
    form->addRow(btnAlign);

    lay->addWidget(grp);
    return w;
}

QWidget* Widget::createInstallTab()
{
    QWidget* w = new QWidget;
    QVBoxLayout* lay = new QVBoxLayout(w);

    QGroupBox* grpDevice = new QGroupBox("设备状态");
    QFormLayout* formDevice = new QFormLayout(grpDevice);
    m_labelDeviceInfo = new QLabel("未连接设备");
    formDevice->addRow("设备信息：", m_labelDeviceInfo);

    QPushButton* btnRefresh = makeBtn("刷新设备", "open");
    connect(btnRefresh, &QPushButton::clicked, this, &Widget::checkDevice);
    formDevice->addRow(btnRefresh);
    lay->addWidget(grpDevice);

    QGroupBox* grp = new QGroupBox("安装到手机");
    QFormLayout* form = new QFormLayout(grp);

    m_editInstallApk = new QLineEdit;
    QPushButton* btnSel = makeBtn("选择APK", "apk");
    connect(btnSel, &QPushButton::clicked, this, &Widget::selectInstallApk);
    QHBoxLayout* h = new QHBoxLayout;
    h->addWidget(m_editInstallApk);
    h->addWidget(btnSel);
    form->addRow("APK文件：", h);

    QPushButton* btnInstall = makeBtn("安装到手机", "run");
    connect(btnInstall, &QPushButton::clicked, this, &Widget::doInstall);
    form->addRow(btnInstall);

    lay->addWidget(grp);
    return w;
}

// ==============================================
// 【全自动】选择APK → 自动填输出目录
// ==============================================
void Widget::selectUnpackApk() {
    QString f = QFileDialog::getOpenFileName(this, "选择APK", "", "APK (*.apk)");
    if(!f.isEmpty()) {
        m_editUnpackApk->setText(f);
        QFileInfo info(f);
        QString outDir = info.absolutePath() + "/" + info.baseName();
        m_editUnpackDir->setText(outDir);
    }
}

void Widget::selectUnpackDir() {
}

// ==============================================
// 【全自动】选择目录 → 自动填输出APK
// ==============================================
void Widget::selectPackDir() {
    QString d = QFileDialog::getExistingDirectory(this);
    if(!d.isEmpty()) {
        m_editPackDir->setText(d);
        QString outApk = d + "_out.apk";
        m_editPackOutApk->setText(outApk);
    }
}

void Widget::selectPackApk() {
}

// ==============================================
// 其他选择
// ==============================================
void Widget::selectShellApk() {
    QString f = QFileDialog::getOpenFileName(this, "选择APK", "", "APK (*.apk)");
    if(!f.isEmpty()) m_editShellApk->setText(f);
}
void Widget::selectSignApk() {
    QString f = QFileDialog::getOpenFileName(this, "选择文件", "", "APK/ZIP (*.apk *.zip)");
    if(!f.isEmpty()) m_editSignApk->setText(f);
}
void Widget::selectSignPubKey() {
    QString f = QFileDialog::getOpenFileName(this, "选择公钥", "", "公钥 (*.x509.pem *.pem)");
    if(!f.isEmpty()) m_editSignPubKey->setText(f);
}
void Widget::selectSignPriKey() {
    QString f = QFileDialog::getOpenFileName(this, "选择私钥", "", "私钥 (*.pk8)");
    if(!f.isEmpty()) m_editSignPriKey->setText(f);
}
void Widget::selectAlignApk() {
    QString f = QFileDialog::getOpenFileName(this, "选择APK", "", "APK (*.apk)");
    if(!f.isEmpty()) m_editAlignApk->setText(f);
}
void Widget::selectInstallApk() {
    QString f = QFileDialog::getOpenFileName(this, "选择APK", "", "APK (*.apk)");
    if(!f.isEmpty()) m_editInstallApk->setText(f);
}

// ==============================================
// 解包：自动创建目录
// ==============================================
void Widget::doUnpack() {
    QString apk = m_editUnpackApk->text().trimmed();
    QString dir = m_editUnpackDir->text().trimmed();
    if(apk.isEmpty() || dir.isEmpty()){
        QMessageBox::warning(this,"提示","请选择APK");
        return;
    }

    QDir().mkpath(dir);
    m_proc->start("apktool", {"d", apk, "-o", dir, "-f"});
    logMsg("▶ 开始解包: " + apk);
}

// ==============================================
// 打包：自动创建目录
// ==============================================
void Widget::doPack() {
    QString src = m_editPackDir->text().trimmed();
    QString out = m_editPackOutApk->text().trimmed();
    if(src.isEmpty() || out.isEmpty()){
        QMessageBox::warning(this,"提示","请选择源码目录");
        return;
    }

    QFileInfo info(out);
    QDir().mkpath(info.absolutePath());
    m_proc->start("apktool", {"b", src, "-o", out});
    logMsg("▶ 开始打包: " + src);
}

void Widget::doCheckShell() {
    QString apk = m_editShellApk->text().trimmed();
    if(apk.isEmpty()){ QMessageBox::warning(this,"提示","请选择APK"); return; }

    logMsg("▶ 解析 APK 信息...");

    QProcess p;
    p.start("aapt", {"dump", "badging", apk});
    p.waitForFinished();
    QString info = p.readAllStandardOutput();

    p.start("unzip", {"-l", apk});
    p.waitForFinished();
    QString out = p.readAllStandardOutput();

    QString shell = "✅ 未检测到加壳 (原生未加固)";

    // 主流加固特征
    if (out.contains("jiagu") || out.contains("Qihoo")) {
        shell = "🔒 360加固";
    } else if (out.contains("Tencent") || out.contains("Bugly")) {
        shell = "🔒 腾讯乐固(腾讯加固)";
    } else if (out.contains("SecShell") || out.contains("SecSecurity")) {
        shell = "🔒 爱加密";
    } else if (out.contains("bangcle") || out.contains("Bangcle")) {
        shell = "🔒 梆梆加固";
    } else if (out.contains("BaiduSecurity")) {
        shell = "🔒 百度加固";
    } else if (out.contains("alibaba") || out.contains("AliSecurity")) {
        shell = "🔒 阿里聚安全";
    } else if (out.contains("VProtect")) {
        shell = "🔒 VProtect 加固";
    } else if (out.contains("tianya") || out.contains("TianYa")) {
        shell = "🔒 天涯加固";
    } else if (out.contains("dexencrypted")) {
        shell = "🔒 未知加固 (Dex加密)";
    } else if (out.contains("reinforce") || out.contains("shell")) {
        shell = "🔒 第三方加固 (通用特征)";
    }

    logMsg("\n=====================================");
    logMsg("           📦 APK 基础信息");
    logMsg("=====================================");
    logMsg(info);
    logMsg("=====================================");
    logMsg("           🔍 加固检测结果");
    logMsg("=====================================");
    logMsg(shell);
    logMsg("=====================================\n");
}

void Widget::doSign() {
    QString input = m_editSignApk->text().trimmed();
    QString pub = m_editSignPubKey->text().trimmed();
    QString pri = m_editSignPriKey->text().trimmed();

    if (input.isEmpty() || pub.isEmpty() || pri.isEmpty()) {
        QMessageBox::warning(this, "提示", "请选择文件、公钥、私钥");
        return;
    }

    // 输出文件自动命名
    QString output;
    if (input.endsWith(".zip")) {
        output = input.chopped(4) + "_signed.zip";
    } else {
        output = input.chopped(4) + "_signed.apk";
    }

    // 自动创建输出目录
    QFileInfo info(output);
    QDir().mkpath(info.absolutePath());

    // ====================== 【正确顺序】signapk 命令参数 ======================
    QStringList args;

    // 1. 选项参数（最前面）
    if (m_cbSignWhole->isChecked())  args << "-w";
    if (m_cbDisableV2->isChecked())  args << "--disable-v2";
    if (m_cbEnableV4->isChecked())   args << "--enable-v4";
    args << "--min-sdk-version" << m_editMinSdk->text().trimmed();
    args << "-a" << m_editAlign->text().trimmed();

    // 2. 密钥（必须放中间）
    args << pub << pri;

    // 3. 输入输出（放最后！）
    args << input << output;

    m_proc->start("signapk", args);
    logMsg("▶ 开始签名: " + input);
}


void Widget::doAlign() {
    QString apk = m_editAlignApk->text().trimmed();
    if(apk.isEmpty()){ QMessageBox::warning(this,"提示","请选择APK"); return; }
    QString out = apk + ".align.apk";
    m_proc->start("zipalign", {"-f", "4", apk, out});
    logMsg("▶ 对齐优化: " + apk);
}

void Widget::doInstall() {
    QString apk = m_editInstallApk->text().trimmed();
    if(apk.isEmpty()){ QMessageBox::warning(this,"提示","请选择APK"); return; }
    m_proc->start("adb", {"install", "-r", apk});
    logMsg("▶ 安装到手机: " + apk);
}

void Widget::checkDevice() {
    QProcess p;
    p.start("adb", {"devices", "-l"});
    p.waitForFinished();
    QString out = p.readAllStandardOutput();

    if (out.contains("device")) {
        p.start("adb", {"shell", "getprop", "ro.product.model"});
        p.waitForFinished();
        QString model = p.readAllStandardOutput().trimmed();

        p.start("adb", {"shell", "getprop", "ro.build.version.release"});
        p.waitForFinished();
        QString ver = p.readAllStandardOutput().trimmed();

        m_labelDeviceInfo->setText(QString("已连接 → %1 | 安卓版本: %2").arg(model).arg(ver));
    } else {
        m_labelDeviceInfo->setText("未连接设备");
    }
}
