#include "sortingtab.h"
#include "ui_sortingtab.h"

SortingTab::SortingTab(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SortingTab)
{
    ui->setupUi(this);
    sortingScene = new QGraphicsScene(this);
    ui->sortingGraphicsView->setScene(sortingScene);
    connect(this, &SortingTab::windowResizedSignal, this, &SortingTab::windowResizedSlot);

    ui->backgroundColorButton->setStyleSheet("QPushButton { background-color : #000000; color : #ffffff; }");
    ui->columnColorButton->setStyleSheet("QPushButton { background-color : #0000ff; color : #ffffff; }");
    ui->highlightColorButton->setStyleSheet("QPushButton { background-color : #00ff00; color : #000000; }");

    currentNumberOfItems = 100;
    numbers = new int[currentNumberOfItems];
    generateLinearDistribution();
}

SortingTab::~SortingTab()
{
    delete ui;
    delete[] numbers;
}

void SortingTab::renderData(const int* items,const int numberOfItems,const int green) const
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
        sortingScene->addLine(penWidth*(i+1) + 10, ui->sortingGraphicsView->height() - items[i] * ySize/numberOfItems - 10, penWidth*(i+1) + 10, ui->sortingGraphicsView->size().height() - 10, outlinePen);
        outlinePen.setColor(columnColor);
    }
//    on_pushButton_clicked();
}

void SortingTab::render() const { renderData(numbers, currentNumberOfItems); }

void SortingTab::render(const int green) const { renderData(numbers, currentNumberOfItems, green); }

void SortingTab::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    emit windowResizedSlot();
}

void SortingTab::shuffleNumbers() const
{
    std::shuffle(&numbers[0],&numbers[currentNumberOfItems],std::mt19937{std::random_device{}()});
    render();
}

void SortingTab::on_runSortingAlgorithmButton_clicked()
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
        case 3:
            selectionSort(numbers,currentNumberOfItems);
        break;
        case 4:
            insertionSort(numbers,currentNumberOfItems);
        break;
        case 5:
            radixSort(numbers,currentNumberOfItems);
        break;
        case 6:
            cocktailSort(numbers,currentNumberOfItems);
        break;
    }
    qInfo() << "done";

    ui->runSortingAlgorithmButton->setDisabled(false);
    ui->shuffleButton->setDisabled(false);
    render();
}

void SortingTab::windowResizedSlot() const
{
    sortingScene->setSceneRect(5,5,ui->sortingGraphicsView->size().width()-5,ui->sortingGraphicsView->size().height()-5);
    render();
}

void SortingTab::on_numberOfItemsSpinBox_valueChanged(const int arg1)
{
    if(currentNumberOfItems!=arg1){
        currentNumberOfItems = arg1;
        if (numbers) delete[] numbers;
        numbers = new int[currentNumberOfItems];
        if (distributionType==0) generateLinearDistribution();
        else if (distributionType==1) generateRandomDistribution();
    }
}

void SortingTab::on_shuffleButton_clicked() const
{
    shuffleNumbers();
}

void SortingTab::on_backgroundColorButton_clicked()
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


void SortingTab::on_columnColorButton_clicked()
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

void SortingTab::on_highlightColorButton_clicked()
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

void SortingTab::on_resetColorsButton_clicked()
{
    backgroundColor=Qt::black;;
    columnColor=Qt::blue;;
    highlightColor=Qt::green;
    ui->backgroundColorButton->setStyleSheet("QPushButton { background-color : #000000; color : #ffffff; }");
    ui->columnColorButton->setStyleSheet("QPushButton { background-color : #0000ff; color : #ffffff; }");
    ui->highlightColorButton->setStyleSheet("QPushButton { background-color : #00ff00; color : #000000; }");
    render();
}

QColor SortingTab::getIdealTextColor(const QColor& rBackgroundColor) const
{
    const int THRESHOLD = 105;
    int BackgroundDelta = (rBackgroundColor.red() * 0.299) + (rBackgroundColor.green() * 0.587) + (rBackgroundColor.blue() * 0.114);
    return QColor((255- BackgroundDelta < THRESHOLD) ? Qt::black : Qt::white);
}

void SortingTab::on_distributionComboBox_activated(const int index)
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

void SortingTab::generateLinearDistribution()
{
    for (int i=0 ; i<currentNumberOfItems ; i++) {
        numbers[i]=i+1;
    }
    shuffleNumbers();
}

void SortingTab::generateRandomDistribution()
{
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> distr(1,currentNumberOfItems);
    for (int i=0 ; i<currentNumberOfItems ; i++) {
        numbers[i] = distr(rng);
    }
    render();
}

void SortingTab::on_pushButton_clicked() const
{
    QImage img(sortingScene->width(),sortingScene->height(),QImage::Format_RGB16);
    QPainter painter(&img);
    ui->sortingGraphicsView->render(&painter);
    QString filename = QString("images/frame") + QString::number(currentFrame++) + QString(".png");
    img.save(filename);
}

//SORTING

void SortingTab::swap(int *a, int *b)
{
    int t = std::move(*a);
    *a = std::move(*b);
    *b = std::move(t);
}

//QUICKSORT

int SortingTab::quickSortPartition(int *arr, const int low, const int high)
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

void SortingTab::quickSort(int *arr, const int low, const int high)
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

void SortingTab::merge(int *array, const int low, const int mid, const int high)
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

void SortingTab::mergeSort(int *array, const int low, const int high)
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

void SortingTab::bubbleSort(int* arr, const int n)
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
        }
    }
}

//SELECTIONSORT

void SortingTab::selectionSort(int* arr, const int n)
{
    int i, j, min_idx;

    // One by one move boundary of unsorted subarray
    for (i = 0; i < n-1; i++)
    {
        // Find the minimum element in unsorted array
        min_idx = i;
        for (j = i+1; j < n; j++)
        {
            if (arr[j] < arr[min_idx]) min_idx = j;
        }
        // Swap the found minimum element with the first element
        swap(&arr[min_idx], &arr[i]);
        std::unique_ptr<QEventLoop> l = std::make_unique<QEventLoop>();
        render(min_idx);
        l->processEvents();
        if (currentNumberOfItems < 250) std::this_thread::sleep_for(std::chrono::milliseconds(20));
    }
}

//INSERSTIONSORT

void SortingTab::insertionSort(int* arr, const int n)
{
    int i, key, j;
    for (i = 1; i < n; i++)
    {
        key = arr[i];
        j = i - 1;

        /* Move elements of arr[0..i-1], that are
        greater than key, to one position ahead
        of their current position */
        while (j >= 0 && arr[j] > key)
        {
            arr[j + 1] = arr[j];
            std::unique_ptr<QEventLoop> l = std::make_unique<QEventLoop>();
            render(j);
            l->processEvents();
            if (currentNumberOfItems < 250) std::this_thread::sleep_for(std::chrono::milliseconds(10));
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

//RADIXSORT

// A function to do counting sort of arr[] according to
// the digit represented by exp.
void SortingTab::countSort(int* arr, const int n, const int exp)
{
    int* output = new int[n]; // output array
    int i, count[10] = { 0 };

    // Store count of occurrences in count[]
    for (i = 0; i < n; i++)
        count[(arr[i] / exp) % 10]++;

    // Change count[i] so that count[i] now contains actual
    //  position of this digit in output[]
    for (i = 1; i < 10; i++)
        count[i] += count[i - 1];

    // Build the output array
    for (i = n - 1; i >= 0; i--) {
        output[count[(arr[i] / exp) % 10] - 1] = arr[i];
        count[(arr[i] / exp) % 10]--;
    }

    // Copy the output array to arr[], so that arr[] now
    // contains sorted numbers according to current digit
    for (i = 0; i < n; i++)
    {
        arr[i] = output[i];
        std::unique_ptr<QEventLoop> l = std::make_unique<QEventLoop>();
        render(i);
        l->processEvents();
        if (currentNumberOfItems < 250) std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

}

// The main function to that sorts arr[] of size n using
// Radix Sort
void SortingTab::radixSort(int* arr, const int n)
{
    // Find the maximum number to know number of digits
    int m = *std::max_element(arr, arr+n);

    // Do counting sort for every digit. Note that instead
    // of passing digit number, exp is passed. exp is 10^i
    // where i is current digit number
    for (int exp = 1; m / exp > 0; exp *= 10)
        countSort(arr, n, exp);
}

//COCKTAILSORT

void SortingTab::cocktailSort(int* a, const int n)
{
    bool swapped = true;
    int start = 0;
    int end = n - 1;

    while (swapped)
    {
        // reset the swapped flag on entering
        // the loop, because it might be true from
        // a previous iteration.
        swapped = false;

        // loop from left to right same as
        // the bubble sort
        for (int i = start; i < end; ++i)
        {
            if (a[i] > a[i + 1]) {
                swap(&a[i], &a[i + 1]);
                std::unique_ptr<QEventLoop> l = std::make_unique<QEventLoop>();
                render(i+1);
                l->processEvents();
                swapped = true;
            }
        }

        // if nothing moved, then array is sorted.
        if (!swapped)
            break;

        // otherwise, reset the swapped flag so that it
        // can be used in the next stage
        swapped = false;

        // move the end point back by one, because
        // item at the end is in its rightful spot
        --end;

        // from right to left, doing the
        // same comparison as in the previous stage
        for (int i = end - 1; i >= start; --i)
        {
            if (a[i] > a[i + 1]) {
                swap(&a[i], &a[i + 1]);
                std::unique_ptr<QEventLoop> l = std::make_unique<QEventLoop>();
                render(i);
                l->processEvents();
                swapped = true;
            }
        }

        // increase the starting point, because
        // the last stage would have moved the next
        // smallest number to its rightful spot.
        ++start;
    }
}
