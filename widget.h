#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTabWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <QProcess>
#include <QPixmap>
#include <QCheckBox>

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    QWidget* createUnpackPackTab();
    QWidget* createInfoShellTab();
    QWidget* createSignTab();
    QWidget* createAlignTab();
    QWidget* createInstallTab();
    QPushButton* makeBtn(const QString& txt, const QString& iconName);

private slots:
    // 解包打包
    void selectUnpackApk();
    void selectUnpackDir();
    void doUnpack();
    void selectPackDir();
    void selectPackApk();
    void doPack();

    // 查壳
    void selectShellApk();
    void doCheckShell();

    // 签名
    void selectSignApk();
    void doSign();
    void selectSignPubKey();
    void selectSignPriKey();

    // 对齐
    void selectAlignApk();
    void doAlign();

    // 安装
    void selectInstallApk();
    void doInstall();
    void checkDevice(); // 新增：查看设备

    // 日志+进程回调
    void logMsg(const QString& txt);
    void onProcOut();
    void onProcEnd(int code);

private:
    QTabWidget* tabWidget;

    // 解包打包
    QLineEdit* m_editUnpackApk;
    QLineEdit* m_editUnpackDir;
    QLineEdit* m_editPackDir;
    QLineEdit* m_editPackOutApk;

    // 查壳
    QLineEdit* m_editShellApk;

    // 签名
    QLineEdit* m_editSignApk;
    QLineEdit* m_editSignPubKey;
    QLineEdit* m_editSignPriKey;
    QCheckBox* m_cbSignWhole;  // <-- 增加这个
    QCheckBox* m_cbDisableV2;
    QCheckBox* m_cbEnableV4;
    QLineEdit* m_editMinSdk;
    QLineEdit* m_editAlign;

    // 对齐
    QLineEdit* m_editAlignApk;

    // 安装
    QLineEdit* m_editInstallApk;
    QLabel* m_labelDeviceInfo;

    // 日志
    QTextEdit* m_logEdit;
    QProcess* m_proc;
};

#endif // WIDGET_H
