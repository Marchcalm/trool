#include <QCoreApplication>
#include <QDebug>
#include <QFile>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(argc == 4 && a.arguments().at(2) == QLatin1String("-o")) {
        QString input = a.arguments().at(1);
        QString output = a.arguments().at(3);
        if (!QFile::exists(input)) {
            qDebug("%s is not exist!", input.toLocal8Bit().constData());
            return 1;
        }
        QFile inFile(input);
        if (!inFile.open(QIODevice::ReadOnly)) {
            qDebug("Can not open %s!", input.toLocal8Bit().constData());
            return 1;
        }
        QFile outFile(output);
        if (!outFile.open(QIODevice::WriteOnly)) {
            qDebug("Can not open %s!", output.toLocal8Bit().constData());
            return 1;
        }

        QStringList codeList;
        while (!inFile.atEnd()) {
            QString line = inFile.readLine();
            QString header = "<source>";
            QString tail = "</source>";
            bool hasHeader = line.contains(header);
            bool hasTail = line.contains(tail);

            if (!hasHeader && !hasTail) {
                continue;
            } else {
                QString code = line.trimmed();
                if (hasHeader) code.replace(header, "");
                if (hasTail) code.replace(tail, "");
                if (code.contains("&")) {
                    if (code.contains("&lt;")) code.replace("&lt;", "<");
                    if (code.contains("&gt;")) code.replace("&gt;", ">");
                    if (code.contains("&quot;")) code.replace("&quot;", "\"");
                    if (code.contains("&apos;")) code.replace("&apos;", "\'");
                    if (code.contains("&amp;")) code.replace("&amp;", "&");
                }

                // remove duplication
                if (!codeList.contains(code)) {
                    codeList.append(code);
                    outFile.write(code.append("\r\n").toUtf8());
                }
            }
        }
        inFile.close();
        outFile.close();
    } else {
        qDebug("example: trool xxx -o xxx");
    }

    return 0;
}
