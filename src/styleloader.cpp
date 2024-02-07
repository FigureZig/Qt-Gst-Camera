#include "styleloader.h"



namespace StyleLoader {

// Load function reads a style sheet from a given file path and returns it as a QString.
QString Load(const QString& filepath) {
    // Attempt to open the file at the provided file path for reading.
    QFile file(filepath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        // If the file cannot be opened, log a debug message and return an empty string.
        qDebug("Style sheet is missing");
        return {};
    }

    // Create a QTextStream to read from the file.
    QTextStream ts(&file);
    // Use QTextStream to read the entire contents of the file into a QString.
    QString styleSheet = ts.readAll();

    // Return the read style sheet as a QString.
    return styleSheet;
}

} // End of StyleLoader namespace
