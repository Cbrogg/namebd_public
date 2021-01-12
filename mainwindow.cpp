#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QStringList stringList, personList;

    QFile file_nac(QCoreApplication::applicationDirPath() + "\\nac.txt");
    if (!file_nac.open(QIODevice::ReadOnly))
    {
        QMessageBox::information(this, QString::fromUtf8("Внимание"), QString::fromUtf8("Отсутствует файл nac.txt!"));
        nac = false;
    }
    else
    {
        QTextStream textStream(&file_nac);
        while (true)
        {
            QString line = textStream.readLine();
            if (line.isNull())
                break;
            else
                stringList.append(line);
        }
        file_nac.close();
        nac = true;
        ui->listWidget_nac->addItems(stringList);
        ui->pushButton_new->setEnabled(true);
    }

    QFile file_person(QCoreApplication::applicationDirPath() + "\\person.txt");
    if (!file_person.open(QIODevice::ReadOnly))
    {
        file_person.open(QIODevice::WriteOnly);
        file_person.close();
    }
    else
    {
        QTextStream textStream_person(&file_person);
        while (true)
        {
            QString line = textStream_person.readLine();
            if (line.isNull())
                break;
            else
                personList.append(line.split("\t")[0]);
        }
        file_person.close();
        nac = true;
        state = 1;
        ui->listWidget_names->addItems(personList);
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_new_clicked()
{
    if (!ui->pushButton_save->isEnabled())
    {
        ui->pushButton_new->setEnabled(false);
        ui->listWidget_names->addItem("Новая запись");
        ui->listWidget_names->setCurrentItem(ui->listWidget_names->item(ui->listWidget_names->count()-1));
        ui->pushButton_save->setEnabled(true);
        ui->pushButton_cansel->setEnabled(true);
        ui->lineEdit_name->setEnabled(true);
        ui->lineEdit_name->setText("");
        ui->radioButton_male->setEnabled(true);
        ui->radioButton_male->setChecked(true);
        ui->radioButton_female->setEnabled(true);
        ui->radioButton_female->setChecked(false);
        ui->spinBox_age->setEnabled(true);
        ui->spinBox_age->setValue(0);
        ui->doubleSpinBox_height->setEnabled(true);
        ui->doubleSpinBox_height->setValue(0);
        ui->doubleSpinBox_weight->setEnabled(true);
        ui->doubleSpinBox_weight->setValue(0);
        ui->checkBox_isAlive->setEnabled(true);
        ui->checkBox_isAlive->setChecked(true);
        ui->dateEdit_birth->setEnabled(true);
        QDate blank(2000,1,1);
        ui->dateEdit_birth->setDate(blank);
        ui->dateEdit_death->setDate(blank);
        ui->listWidget_nac->setEnabled(true);
        ui->listWidget_nac->setCurrentRow(0);
        ui->listWidget_nac->scrollToItem(ui->listWidget_nac->currentItem());
        ui->listWidget_names->setEnabled(false);
        ui->pushButton_del->setEnabled(false);
        ui->pushButton_edit->setEnabled(false);
        state = 2;
    }
}

void MainWindow::on_pushButton_cansel_clicked()
{
    if (ui->pushButton_cansel->isEnabled())
    {
        QMessageBox::StandardButton reply;

        reply = QMessageBox::question(this, QString::fromUtf8("Отмена изменений"), QString::fromUtf8("Подтвердите отмену изменений"), QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            ui->pushButton_save->setEnabled(false);
            ui->pushButton_cansel->setEnabled(false);
            ui->lineEdit_name->setEnabled(false);
            ui->radioButton_male->setEnabled(false);
            ui->radioButton_female->setEnabled(false);
            ui->spinBox_age->setEnabled(false);
            ui->doubleSpinBox_height->setEnabled(false);
            ui->doubleSpinBox_weight->setEnabled(false);
            ui->checkBox_isAlive->setChecked(true);
            ui->checkBox_isAlive->setEnabled(false);
            ui->dateEdit_birth->setEnabled(false);
            ui->listWidget_nac->setEnabled(false);
            ui->dateEdit_death->setEnabled(false);
            if (state == 2)
                delete ui->listWidget_names->currentItem();
            ui->listWidget_names->setEnabled(true);
            ui->pushButton_new->setEnabled(true);
            on_listWidget_names_currentItemChanged(ui->listWidget_names->currentItem());
            state = 1;
        }
        if (ui->listWidget_names->count() < 1)
        {
            ui->pushButton_del->setEnabled(false);
            ui->pushButton_edit->setEnabled(false);
            ui->lineEdit_name->setText("");
            ui->spinBox_age->setValue(0);
            ui->radioButton_male->setChecked(true);
            ui->radioButton_female->setChecked(false);
            ui->doubleSpinBox_height->setValue(0);
            ui->doubleSpinBox_weight->setValue(0);
            ui->checkBox_isAlive->setChecked(true);
            QDate blank(2000,1,1);
            ui->dateEdit_birth->setDate(blank);
            ui->dateEdit_death->setDate(blank);
            ui->listWidget_nac->setCurrentRow(0);
            ui->listWidget_nac->scrollToItem(ui->listWidget_nac->currentItem());
        }
    }
}

void MainWindow::on_checkBox_isAlive_clicked()
{
    ui->dateEdit_death->setEnabled(!ui->checkBox_isAlive->isChecked());
    ui->spinBox_age->setEnabled(!ui->spinBox_age->isEnabled());
}

void MainWindow::on_pushButton_del_clicked()
{
    if (ui->listWidget_names->count() > 0 && ui->listWidget_names->currentItem())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, QString::fromUtf8("Удаление записи"),
                              QString::fromUtf8("Подтвердите удаление записи ") + ui->listWidget_names->currentItem()->text(),
                              QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            QFile file_person(QCoreApplication::applicationDirPath() + "\\person.txt");
            file_person.open(QIODevice::ReadOnly);
            QStringList personList;
            QTextStream textStream_person(&file_person);

            while(true)
            {
            QString line = textStream_person.readLine();
            if (line.isNull())
                break;
            else
                personList.append(line);
            }
            file_person.close();
            file_person.open(QIODevice::WriteOnly);
            for(int i = 0; i < ui->listWidget_names->count(); i++)
            {
                if (i != ui->listWidget_names->currentRow())
                    textStream_person << personList[i] << "\n";
            }
            state = 1;
            delete ui->listWidget_names->currentItem();
        }
        on_listWidget_names_currentItemChanged(ui->listWidget_names->currentItem());
    }
    if (ui->listWidget_names->count() < 1)
    {
        ui->pushButton_del->setEnabled(false);
        ui->pushButton_edit->setEnabled(false);
        ui->lineEdit_name->setText("");
        ui->spinBox_age->setValue(0);
        ui->radioButton_male->setChecked(true);
        ui->radioButton_female->setChecked(false);
        ui->doubleSpinBox_height->setValue(0);
        ui->doubleSpinBox_weight->setValue(0);
        ui->checkBox_isAlive->setChecked(true);
        QDate blank(2000,1,1);
        ui->dateEdit_birth->setDate(blank);
        ui->dateEdit_death->setDate(blank);
        ui->listWidget_nac->setCurrentRow(0);
        ui->listWidget_nac->scrollToItem(ui->listWidget_nac->currentItem());
    }
}

void MainWindow::on_listWidget_names_currentItemChanged(QListWidgetItem *current)
{
    if (current)
    {
        ui->pushButton_del->setEnabled(true);
        ui->pushButton_edit->setEnabled(true);

        QFile file_person(QCoreApplication::applicationDirPath() + "\\person.txt");
        file_person.open(QIODevice::ReadOnly);
        QTextStream textStream_person(&file_person);
        while (true)
        {
            QString line = textStream_person.readLine();
            if (line.isNull())
                break;
            else
                if (line.split("\t")[0] == current->text())
                {
                    QStringList person = line.split("\t");
                    ui->lineEdit_name->setText(person[0]);
                    ui->spinBox_age->setValue(person[1].toInt());
                    if (person[2].toInt() == 1)
                        ui->checkBox_isAlive->setChecked(true);
                    else
                        ui->checkBox_isAlive->setChecked(false);
                    QDate birth(person[3].split(".")[2].toInt(),person[3].split(".")[1].toInt(),person[3].split(".")[0].toInt());
                    ui->dateEdit_birth->setDate(birth);
                    QDate death(person[4].split(".")[2].toInt(),person[4].split(".")[1].toInt(),person[4].split(".")[0].toInt());
                    ui->dateEdit_death->setDate(death);
                    double height = person[5].toDouble();
                    ui->doubleSpinBox_height->setValue(height);
                    double weight = person[6].toDouble();
                    ui->doubleSpinBox_weight->setValue(weight);
                    if (person[7].toInt() == 1)
                    {
                        ui->radioButton_male->setChecked(true);
                        ui->radioButton_female->setChecked(false);
                    }
                    else
                    {
                        ui->radioButton_male->setChecked(false);
                        ui->radioButton_female->setChecked(true);
                    }
                    ui->listWidget_nac->setCurrentRow(person[9].toInt());
                    ui->listWidget_nac->scrollToItem(ui->listWidget_nac->currentItem());
                }
        }
        file_person.close();
    }
    else
    {
        ui->pushButton_del->setEnabled(false);
        ui->pushButton_edit->setEnabled(false);
    }
}

void MainWindow::on_pushButton_save_clicked()
{
    if (ui->pushButton_cansel->isEnabled())
    {
        if (!ui->checkBox_isAlive->isChecked())
        {
            QString brt = ui->dateEdit_birth->text();
            QString dth = ui->dateEdit_death->text();
            QDate birth(brt.split(".")[2].toInt()+ui->spinBox_age->text().toInt(),brt.split(".")[1].toInt(),brt.split(".")[0].toInt());
            QDate birth2(brt.split(".")[2].toInt()+ui->spinBox_age->text().toInt()+1,brt.split(".")[1].toInt(),brt.split(".")[0].toInt());
            QDate death(dth.split(".")[2].toInt(),dth.split(".")[1].toInt(),dth.split(".")[0].toInt());
            if (death < birth || death >= birth2)
            {
                QMessageBox::information(this, QString::fromUtf8("Внимание"), QString::fromUtf8("Дата смерти некорректна!"));
                return;
            }
        }
        QString restr = "! @ # $ % ^ & * ( ) - _ = + [ ] { } , . < > / ? \\ * \" № ; :";
        QStringList rst = restr.split(" ");

        QStringList name = ui->lineEdit_name->text().split(" ");
        for (int i = 0; i < name.count(); i++)
        {
            for (int j = 0; j < rst.count(); j++)
            {
                if (name[i].contains(rst[j]))
                {
                    QMessageBox::information(this, QString::fromUtf8("Внимание"), QString::fromUtf8("ФИО некорректно!"));
                    return;
                }
            }
        }

        QFile file_person(QCoreApplication::applicationDirPath() + "\\person.txt");
        if (state == 2)
        {
            QString dth, gender;
            if (ui->checkBox_isAlive->isChecked())
                dth = QString::fromUtf8("");
            else
                dth = QString::fromUtf8("Дата смерти:\t\t") + ui->dateEdit_death->text() + QString::fromUtf8("\n");
            if (ui->radioButton_male->isChecked())
                gender = QString::fromUtf8("Пол:\t\t\tМужской\n");
            else
                gender = QString::fromUtf8("Пол:\t\t\tЖенский\n");
            QString msg =
                    QString::fromUtf8("ФИО:\t\t\t")             +  ui->lineEdit_name->text()                 + QString::fromUtf8("\n") +
                    QString::fromUtf8("Возраст:\t\t\t")         +  ui->spinBox_age->text()                   + QString::fromUtf8("\n") +
                    QString::fromUtf8("Дата рождения:\t\t")     +  ui->dateEdit_birth->text()                + QString::fromUtf8("\n") +
                    dth +
                    QString::fromUtf8("Рост:\t\t\t")            +  ui->doubleSpinBox_height->text()          + QString::fromUtf8("\n") +
                    QString::fromUtf8("Вес:\t\t\t")             +  ui->doubleSpinBox_weight->text()          + QString::fromUtf8("\n") +
                    gender +
                    QString::fromUtf8("Национальность:\t")    +  ui->listWidget_nac->currentItem()->text() + QString::fromUtf8("\n");

            QMessageBox::StandardButton reply;

            reply = QMessageBox::question(this, QString::fromUtf8("Добавление записи"), QString::fromUtf8("Подтвердите введенные данные:\n") + msg, QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                file_person.open(QIODevice::Append);
                QTextStream textStream_person(&file_person);
                textStream_person << ui->lineEdit_name->text() << "\t"
                                    << ui->spinBox_age->text() << "\t"
                                    << ui->checkBox_isAlive->isChecked() << "\t"
                                    << ui->dateEdit_birth->text() << "\t"
                                    << ui->dateEdit_death->text() << "\t"
                                    << ui->doubleSpinBox_height->text().split(",")[0] << "." << ui->doubleSpinBox_height->text().split(",")[1] << "\t"
                                    << ui->doubleSpinBox_weight->text().split(",")[0] << "." << ui->doubleSpinBox_weight->text().split(",")[1] << "\t"
                                    << ui->radioButton_male->isChecked() << "\t"
                                    << ui->listWidget_nac->currentItem()->text() << "\t"
                                    << ui->listWidget_nac->currentRow() << "\n";
                file_person.close();
                ui->pushButton_save->setEnabled(false);
                ui->pushButton_cansel->setEnabled(false);
                ui->lineEdit_name->setEnabled(false);
                ui->radioButton_male->setEnabled(false);
                ui->radioButton_female->setEnabled(false);
                ui->spinBox_age->setEnabled(false);
                ui->doubleSpinBox_height->setEnabled(false);
                ui->doubleSpinBox_weight->setEnabled(false);
                ui->checkBox_isAlive->setChecked(true);
                ui->checkBox_isAlive->setEnabled(false);
                ui->dateEdit_birth->setEnabled(false);
                ui->listWidget_nac->setEnabled(false);
                ui->dateEdit_death->setEnabled(false);
                ui->listWidget_names->setEnabled(true);
                ui->pushButton_del->setEnabled(true);
                ui->pushButton_edit->setEnabled(true);
                ui->listWidget_names->currentItem()->setText(ui->lineEdit_name->text());
                ui->pushButton_new->setEnabled(true);
                state = 1;
            }
        }
        if (state == 3)
        {
            QMessageBox::StandardButton reply;

            reply = QMessageBox::question(this, QString::fromUtf8("Изменение записи"), QString::fromUtf8("Подтвердите сохранение изменений"), QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                file_person.open(QIODevice::ReadOnly);
                QStringList personList;
                QTextStream textStream_person(&file_person);

                while(true)
                {
                QString line = textStream_person.readLine();
                if (line.isNull())
                    break;
                else
                    personList.append(line);
                }
                file_person.close();
                file_person.open(QIODevice::WriteOnly);
                for(int i = 0; i < ui->listWidget_names->count(); i++)
                {
                    if (i == ui->listWidget_names->currentRow())
                    {
                        textStream_person << ui->lineEdit_name->text() << "\t"
                                            << ui->spinBox_age->text() << "\t"
                                            << ui->checkBox_isAlive->isChecked() << "\t"
                                            << ui->dateEdit_birth->text() << "\t"
                                            << ui->dateEdit_death->text() << "\t"
                                            << ui->doubleSpinBox_height->text().split(",")[0] << "." << ui->doubleSpinBox_height->text().split(",")[1] << "\t"
                                            << ui->doubleSpinBox_weight->text().split(",")[0] << "." << ui->doubleSpinBox_weight->text().split(",")[1] << "\t"
                                            << ui->radioButton_male->isChecked() << "\t"
                                            << ui->listWidget_nac->currentItem()->text() << "\t"
                                            << ui->listWidget_nac->currentRow() << "\n";
                    }
                    else
                        textStream_person << personList[i] << "\n";

                }
                file_person.close();
                ui->pushButton_save->setEnabled(false);
                ui->pushButton_cansel->setEnabled(false);
                ui->lineEdit_name->setEnabled(false);
                ui->radioButton_male->setEnabled(false);
                ui->radioButton_female->setEnabled(false);
                ui->spinBox_age->setEnabled(false);
                ui->doubleSpinBox_height->setEnabled(false);
                ui->doubleSpinBox_weight->setEnabled(false);
                ui->checkBox_isAlive->setChecked(true);
                ui->checkBox_isAlive->setEnabled(false);
                ui->dateEdit_birth->setEnabled(false);
                ui->listWidget_nac->setEnabled(false);
                ui->dateEdit_death->setEnabled(false);
                ui->listWidget_names->setEnabled(true);
                ui->pushButton_del->setEnabled(true);
                ui->pushButton_edit->setEnabled(true);
                ui->listWidget_names->currentItem()->setText(ui->lineEdit_name->text());
                ui->pushButton_new->setEnabled(true);
                state = 1;
            }
        }
    }
}

void MainWindow::on_pushButton_edit_clicked()
{
    ui->pushButton_new->setEnabled(false);
    ui->pushButton_save->setEnabled(true);
    ui->pushButton_cansel->setEnabled(true);
    ui->lineEdit_name->setEnabled(true);
    ui->radioButton_male->setEnabled(true);
    ui->radioButton_female->setEnabled(true);
    ui->spinBox_age->setEnabled(true);
    ui->doubleSpinBox_height->setEnabled(true);
    ui->doubleSpinBox_weight->setEnabled(true);
    ui->checkBox_isAlive->setEnabled(true);
    ui->dateEdit_birth->setEnabled(true);
    ui->listWidget_nac->setEnabled(true);
    ui->listWidget_names->setEnabled(false);
    ui->pushButton_del->setEnabled(false);
    ui->pushButton_edit->setEnabled(false);

    if (!ui->checkBox_isAlive->isChecked())
    {
        ui->dateEdit_death->setEnabled(true);

    }

    state = 3;
}

void MainWindow::on_listWidget_names_itemDoubleClicked()
{
    on_pushButton_edit_clicked();
}

void MainWindow::on_dateEdit_death_dateChanged()
{
    if (!ui->checkBox_isAlive->isChecked())
    {
        int age = ui->dateEdit_death->text().split(".")[2].toInt()-ui->dateEdit_birth->text().split(".")[2].toInt();
        if (ui->dateEdit_death->text().split(".")[1].toInt() < ui->dateEdit_birth->text().split(".")[1].toInt())
            age--;
        else if (ui->dateEdit_death->text().split(".")[1].toInt() == ui->dateEdit_birth->text().split(".")[1].toInt() &&
                 ui->dateEdit_death->text().split(".")[0].toInt() < ui->dateEdit_birth->text().split(".")[0].toInt())
            age--;
        ui->spinBox_age->setValue(age);
    }
}
