#include "sortingtab.h"
#include "ui_sortingtab.h"

sortingTab::sortingTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::sortingTab)
{
    ui->setupUi(this);
    sortingScene = new QGraphicsScene(this);
    ui->sortingGraphicsView->setScene(sortingScene);
    connect(this, &sortingTab::windowResizedSignal, this, &sortingTab::windowResizedSlot);

    ui->backgroundColorButton->setStyleSheet("QPushButton { background-color : #000000; color : #ffffff; }");
    ui->columnColorButton->setStyleSheet("QPushButton { background-color : #0000ff; color : #ffffff; }");
    ui->highlightColorButton->setStyleSheet("QPushButton { background-color : #00ff00; color : #000000; }");

    currentNumberOfItems = 100;
    numbers = new int[currentNumberOfItems];
    generateLinearDistribution();
}

sortingTab::~sortingTab()
{
    delete ui;
}

void sortingTab::renderData(const int* items,const int numberOfItems,const int green) const
{
    sortingScene->clear();
    QBrush backgroundBrush(backgroundColor);
    ui->sortingGraphicsView->setBackgroundBrush(backgroundBrush);

    QPen outlinePen(columnColor);
    double penWidth = ((ui->sortingGraphicsView->size().width())-15)/(double(numberOfItems+1));
    int ySize = ui->sortingGraphicsView->size().height() - 20 - penWidth;
    outlinePen.setWidthF(penWidth - 1);

    for (int i=0 ; i<currentNumberOfItems ; i++) {
        if (green==i)
        {
            outlinePen.setColor(highlightColor);
        }
        sortingScene->addLine(penWidth*(i+1) + 10, items[i] * ySize/numberOfItems + 10, penWidth*(i+1) + 10, ui->sortingGraphicsView->size().height() - 10, outlinePen);
        outlinePen.setColor(columnColor);
    }
}

void sortingTab::render() const { renderData(numbers, currentNumberOfItems); }

void sortingTab::render(const int green) const { renderData(numbers, currentNumberOfItems, green); }

void sortingTab::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    emit windowResizedSlot();
}

void sortingTab::shuffleNumbers() const
{
    std::shuffle(&numbers[0],&numbers[currentNumberOfItems],std::mt19937{std::random_device{}()});
    render();
}

void sortingTab::on_runSortingAlgorithmButton_clicked()
{
    ui->runSortingAlgorithmButton->setDisabled(true);
    ui->shuffleButton->setDisabled(true);
    switch (ui->selectedSortingAlgorithmComboBox->currentIndex()) {
        case 0:
            quickSort(numbers,0,currentNumberOfItems-1);
        break;
        case 1:
            mergeSort(numbers,0,currentNumberOfItems-1);
        break;
        case 2:
            bubbleSort(numbers,currentNumberOfItems);
        break;
    }
    qInfo() << "done";

    ui->runSortingAlgorithmButton->setDisabled(false);
    ui->shuffleButton->setDisabled(false);
    render();
}

void sortingTab::windowResizedSlot() const
{
    sortingScene->setSceneRect(5,5,ui->sortingGraphicsView->size().width()-5,ui->sortingGraphicsView->size().height()-5);
    render();
}

void sortingTab::on_numberOfItemsSpinBox_valueChanged(const int arg1)
{
    if(currentNumberOfItems!=arg1){
        currentNumberOfItems = arg1;
        if (numbers) delete[] numbers;
        numbers = new int[currentNumberOfItems];
        if (distributionType==0) generateLinearDistribution();
        else if (distributionType==1) generateRandomDistribution();
    }
}

void sortingTab::on_shuffleButton_clicked() const
{
    shuffleNumbers();
}

void sortingTab::on_backgroundColorButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "Choose Background Color");
    if (color.isValid())
    {
        backgroundColor = color;
        const QString COLOR_STYLE("QPushButton { background-color : %1; color : %2; }");
        QColor IdealTextColor = getIdealTextColor(color);
        ui->backgroundColorButton->setStyleSheet(COLOR_STYLE.arg(color.name()).arg(IdealTextColor.name()));
        render();
    }
}


void sortingTab::on_columnColorButton_clicked()
{
    QColor color =  QColorDialog::getColor(Qt::blue, this, "Choose Column Color");
    if (color.isValid())
    {
        columnColor = color;
        const QString COLOR_STYLE("QPushButton { background-color : %1; color : %2; }");
        QColor IdealTextColor = getIdealTextColor(color);
        ui->columnColorButton->setStyleSheet(COLOR_STYLE.arg(color.name()).arg(IdealTextColor.name()));
        render();
    }

}

void sortingTab::on_highlightColorButton_clicked()
{
    QColor color =  QColorDialog::getColor(Qt::green, this, "Choose Highlight Color");
    if (color.isValid())
    {
        highlightColor = color;
        const QString COLOR_STYLE("QPushButton { background-color : %1; color : %2; }");
        QColor IdealTextColor = getIdealTextColor(color);
        ui->highlightColorButton->setStyleSheet(COLOR_STYLE.arg(color.name()).arg(IdealTextColor.name()));
        render();
    }
}

void sortingTab::on_resetColorsButton_clicked()
{
    backgroundColor=Qt::black;;
    columnColor=Qt::blue;;
    highlightColor=Qt::green;
    ui->backgroundColorButton->setStyleSheet("QPushButton { background-color : #000000; color : #ffffff; }");
    ui->columnColorButton->setStyleSheet("QPushButton { background-color : #0000ff; color : #ffffff; }");
    ui->highlightColorButton->setStyleSheet("QPushButton { background-color : #00ff00; color : #000000; }");
    render();
}

QColor sortingTab::getIdealTextColor(const QColor& rBackgroundColor) const
{
    const int THRESHOLD = 105;
    int BackgroundDelta = (rBackgroundColor.red() * 0.299) + (rBackgroundColor.green() * 0.587) + (rBackgroundColor.blue() * 0.114);
    return QColor((255- BackgroundDelta < THRESHOLD) ? Qt::black : Qt::white);
}

void sortingTab::on_distributionComboBox_activated(const int index)
{
    distributionType = index;
    switch (index) {
        case 0:
            generateLinearDistribution();
        break;
        case 1:
            generateRandomDistribution();
        break;

    }
}

void sortingTab::generateLinearDistribution()
{
    for (int i=0 ; i<currentNumberOfItems ; i++) {
        numbers[i]=i+1;
    }
    shuffleNumbers();
}

void sortingTab::generateRandomDistribution()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distr(1,currentNumberOfItems);
    for (int i=0 ; i<currentNumberOfItems ; i++) {
        numbers[i] = distr(rng);
    }
    render();
}
//SORTING
void sortingTab::swap(int *a, int *b)
{
    int t = std::move(*a);
    *a = std::move(*b);
    *b = std::move(t);
}

//QUICKSORT

int sortingTab::quickSortPartition(int *arr, const int low, const int high)
{
    int pivot = arr[high]; // pivot
    int i = (low - 1); // Index of smaller element and indicates the right position of pivot found so far

    for (int j = low; j <= high - 1; j++)
    {
        // If current element is smaller than the pivot
        if (arr[j] < pivot)
        {
            i++; // increment index of smaller element
            swap(&arr[i], &arr[j]);

            std::unique_ptr<QEventLoop> l = std::make_unique<QEventLoop>();
            render(j);
            l->processEvents();
            if (currentNumberOfItems < 250) std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    swap(&arr[i + 1], &arr[high]);

    std::unique_ptr<QEventLoop> l = std::make_unique<QEventLoop>();
    render(i);
    l->processEvents();

    return (i + 1);
}

void sortingTab::quickSort(int *arr, const int low, const int high)
{
    if (low < high)
       {
           /* pi is partitioning index, arr[p] is now
           at right place */
           int pi = quickSortPartition(arr, low, high);
           // Separately sort elements before
           // partition and after partition
           quickSort(arr, low, pi - 1);
           quickSort(arr, pi + 1, high);
       }
}

//MERGESORT

void sortingTab::merge(int *array, const int low, const int mid, const int high)
{
    int *temp = new int[high + 1];
    int i = low;
    int j = mid + 1;
    int k = 0;

    while (i <= mid && j <= high)
    {
        if (array[i] <= array[j])
            temp[k++] = array[i++];
        else
            temp[k++] = array[j++];
    }
    while (i <= mid)
        temp[k++] = array[i++];
    while (j <= high)
        temp[k++] = array[j++];
    k--;
    while (k >= 0)
    {
        array[k + low] = temp[k];
        k--;
        std::unique_ptr<QEventLoop> l = std::make_unique<QEventLoop>();
        render(k+low);
        l->processEvents();
        if (currentNumberOfItems < 250) std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    delete[] temp;
}

void sortingTab::mergeSort(int *array, const int low, const int high)
{
    int mid;

    if (low < high)
    {
        mid = (low + high) / 2;
        std::unique_ptr<QEventLoop> l = std::make_unique<QEventLoop>();
        render(mid);
        l->processEvents();
        mergeSort(array, low, mid);
        mergeSort(array, mid + 1, high);
        merge(array, low, mid, high);
    }
}

//BUBBLESORT

void sortingTab::bubbleSort(int* arr, const int n)
{
    int i, j;
    for (i = 0; i < n-1; i++)
    {
        // Last i elements are already in place
        for (j = 0; j < n-i-1; j++)
        {
            if (arr[j] > arr[j+1]) swap(&arr[j], &arr[j+1]);
            std::unique_ptr<QEventLoop> l = std::make_unique<QEventLoop>();
            render(j);
            l->processEvents();
            if (currentNumberOfItems < 250) std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
}



