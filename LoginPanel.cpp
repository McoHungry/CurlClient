#include "LoginPanel.h"
#include <QMessageBox>
#include "cJSON.h"
#include <curl/curl.h>

LoginPanel::LoginPanel(QWidget *parent) : QWidget(parent)
{
    QPushButton *button;

    QGridLayout *mainLayout = new QGridLayout(this);

    mainLayout->setRowStretch(0,1);
    mainLayout->setColumnStretch(0,1);
    mainLayout->setColumnStretch(3,1);
    mainLayout->setRowStretch(4,1);

    mainLayout->addWidget(new QLabel("Username: "), 1, 1);
    mainLayout->addWidget(new QLabel("Password: "), 2, 1);
    mainLayout->addWidget(_username = new QLineEdit, 1, 2);
    mainLayout->addWidget(_password = new QLineEdit, 2, 2);

    QHBoxLayout* hBox;
    mainLayout->addLayout(hBox = new QHBoxLayout, 3, 1, 1, 2);

    hBox->addStretch(1);
    hBox->addWidget(button = new QPushButton("Login"));

    connect(button, SIGNAL(clicked()),this,
            SLOT(slotButtonClicked()));

    _password->setEchoMode(QLineEdit::Password);
}

size_t wirte_callback(char* ptr, size_t size, size_t nmemb, void* userdata)
{
    QByteArray *buf = (QByteArray*)userdata;
    buf->append(ptr, size*nmemb);
    return size * nmemb;
}

void LoginPanel::slotButtonClicked()
{
    QString reg = "reg";
    QString username = _username->text();
    QString password = _password->text();
    QString id = "fqwerqwer";
    int age = 18;
    int sex = 1;
    char tel[] = "2312x31213";
    char cardid[] = "12333123123123";

    if (username.length() == 0 || password.length() == 0)
    {
        QMessageBox::warning(this, "error", "username is not empty");
        return;
    }

    char* buf;
    {
        cJSON* root = cJSON_CreateObject();
        cJSON_AddItemToObject(root,"cmd", cJSON_CreateString(reg.toUtf8().data()));
        cJSON_AddItemToObject(root,"username", cJSON_CreateString(username.toUtf8().data()));
        cJSON_AddItemToObject(root,"password", cJSON_CreateString(password.toUtf8().data()));
        cJSON_AddItemToObject(root,"id", cJSON_CreateString(id.toUtf8().data()));
        cJSON_AddItemToObject(root,"age", cJSON_CreateNumber(age));
        cJSON_AddItemToObject(root,"sex", cJSON_CreateNumber(sex));
        cJSON_AddItemToObject(root,"tel", cJSON_CreateString(tel));
        cJSON_AddItemToObject(root,"cardid", cJSON_CreateString(cardid));

        buf = cJSON_Print(root);
        cJSON_Delete(root);
    }

    //CURL* curl = curl_easy_init();
    CURL* curl = curl_easy_init();

    QByteArray responseData;

    curl_easy_setopt(curl, CURLOPT_URL, "127.0.0.1:8080");
    // set post data
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, wirte_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

    CURLcode code = curl_easy_perform(curl);
    if (code != CURLE_OK)
    {
        QMessageBox::warning(this, "error", "perform err");
        curl_easy_cleanup(curl);
        free(buf);
        return;
    }
    curl_easy_cleanup(curl);
    free(buf);

    QMessageBox::warning(this, "responseData", QString(responseData));
    {
        responseData.append('\0');
        cJSON* root = cJSON_Parse(responseData.data());
        cJSON* result = cJSON_GetObjectItem(root, "result");
        if (result == NULL)
        {
            QMessageBox::warning(this,"error","unknown err");
            return;
        }
        if (strcmp(result->valuestring, "ok") == 0)
        {
            QMessageBox::warning(this, "ok", "Login success");
            this->deleteLater();
        }
        else
        {
            QMessageBox::warning(this, "no", "no");
            cJSON* reason = cJSON_GetObjectItem(root, "reason");
            QMessageBox::warning(this, "error", reason->valuestring);
        }
        cJSON_Delete(root);
    }
}
