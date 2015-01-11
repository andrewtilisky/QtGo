#include "MiniGo.h"

MiniGo::MiniGo(QWidget *parent, unsigned char dimensions, unsigned char plies) :
		QMainWindow(parent), primary_node(new GameTreeNode), mPlies(plies), mPass(
				0) {
	primary_node->game_state = new State(dimensions, false);

	ui.setupUi(this);
	custom_view = new BoardView(ui.centralWidget, primary_node->game_state);
	custom_view->setObjectName(QStringLiteral("graphicsView"));
	custom_view->setGeometry(QRect(240, 10, 370, 370));
	custom_view->setScene(new QGraphicsScene(this));
	ui.graphicsView = custom_view;

	mBrush.setStyle(Qt::SolidPattern);

	qDebug("size of Board is %lu", sizeof(Board));
	qDebug("size of State is %lu", sizeof(State));
	qDebug("size of Point is %lu", sizeof(Point));

//	captureTest2();
	custom_view->paint_board(NULL);
//	updateListWidget();
}

MiniGo::~MiniGo() {
	delete primary_node;
}

void MiniGo::updateListWidget() {
	ui.listWidget->clear();
	for (unsigned short state = 0; state < primary_node->subnodes.size();
			++state) {
		// unsigned char x = mPrimaryState->branch (stateIdx).x(),
		// y = mPrimaryState->branch (stateIdx).y();
		const State* branch = new State(primary_node->subnodes.at(state));

		ui.listWidget->addItem(
				QString::number(branch->x()) + ", "
						+ QString::number(branch->y()) + " ("
						+ QString::number(branch->blackScore()) + ")");
	}
	ui.listWidget->setCurrentRow(0);
}

void MiniGo::paintEvent(QPaintEvent *) {
}

/*
 void MiniGo::on_previewButton_clicked()
 {
 unsigned short row = ui.listWidget->currentIndex().row();

 if (activeState == &primaryState->subStateAt (row))
 {
 activeState = primaryState;
 }
 else
 {
 activeState = &primaryState->subStateAt (row);
 }
 mActive_state = mPrimary_state;
 }
 */

void MiniGo::captureTest1() {
	try {
		primary_node->game_state = primary_node->game_state->place(2, 1);
		primary_node->game_state = primary_node->game_state->place(2, 2);
		primary_node->game_state = primary_node->game_state->place(2, 3);
		primary_node->game_state = new State(*primary_node->game_state);
		primary_node->game_state = primary_node->game_state->place(1, 2);
		primary_node->game_state = new State(*primary_node->game_state);
		primary_node->game_state = primary_node->game_state->place(3, 2);
		custom_view->setState(primary_node->game_state);
	} catch (exception& e) {
		ui.statusBar->showMessage(e.what());
	}
}

void MiniGo::captureTest2() {
	try {
		primary_node->game_state = primary_node->game_state->place(3, 0);
		primary_node->game_state = primary_node->game_state->place(4, 4);
		primary_node->game_state = primary_node->game_state->place(3, 1);
		primary_node->game_state = primary_node->game_state->place(4, 3);
		primary_node->game_state = primary_node->game_state->place(3, 2);
		primary_node->game_state = primary_node->game_state->place(4, 2);
		primary_node->game_state = primary_node->game_state->place(3, 3);
		primary_node->game_state = primary_node->game_state->place(4, 1);
		primary_node->game_state = primary_node->game_state->place(3, 4);
//		primary_node->game_state = primary_node->game_state->place(4,0);
		custom_view->setState(primary_node->game_state);
	} catch (exception& e) {
		ui.statusBar->showMessage(e.what());
	}
}

//deleted check for whether or not the primary state's
//board is filled i.e. has stones == dimensions^2
void MiniGo::on_moveButton_clicked() {
	//    unsigned short row = ui.listWidget->currentIndex().row();

	//    ui.statusBar->showMessage("Deleting obsolete states");

	//uint64_t runstarttime = time::GetTimeMs64();

	try {
		primary_node->game_state = primary_node->game_state->place(
				custom_view->lastPointClicked().x,
				custom_view->lastPointClicked().y);
	} catch (exception& e) {
		ui.statusBar->showMessage(e.what());
	}

	mPass = false;

	//primaryState = primaryState->releaseSubStateAt (row);
	//    mActive_state = mPrimaryState;
	custom_view->setState(primary_node->game_state);
	custom_view->paint_board(NULL);

	/*uint64_t runendtime = time::GetTimeMs64();
	 uint64_t runtime = runendtime - runstarttime;*/

	//cout << "deletion time" << runtime << "for" << plies << "plies dimension";
	//        cout << primaryState->getDimensions();
	//    ui.statusBar->showMessage("Generating new ply");
	//    long run_start_time = time::GetTimeMs64();
	//    mPrimaryState->generate_new_level(mPlies);
	//    long run_end_time = time::GetTimeMs64();
	//    long run_time = run_end_time - run_start_time;
	//    qDebug("new ply generation time %d for %i", run_time, mPlies);
	//    qDebug("plies dimension %d", mPrimaryState->dimensions());
	//    ui.statusBar->showMessage("Move made");
//	updateListWidget();
}

void MiniGo::onListWidgetCurrentItemChanged(QListWidgetItem *) {
	unsigned short row = ui.listWidget->currentIndex().row();

	if (row < primary_node->subnodes.size()) {
		ui.statusBar->showMessage("Item " + QString::number(row) + " selected");
		//        mActive_state = &mPrimaryState->branch (row);
		paintEvent(NULL);
	}
}

void MiniGo::on_passButton_clicked() {
	//toggle pass flag to indicate one player's passed
	//end the game someone's just passed and the flag
	//hasn't been reset when a move was made.
	if (not mPass) {
		mPass = not mPass;
	} else {
		QMessageBox::information(NULL, "",
				"Both players have consecutively passed.  The game is over");
		this->close();
	}

	try {
		primary_node->game_state = new State(*primary_node->game_state);
		custom_view->setState(primary_node->game_state);
	} catch (exception& e) {
		ui.statusBar->showMessage(e.what());
	}
}

//refactoring this outside state means it has to
//grow the tree and keep track of the minmax
//heuristic.  this might be easier with some stuct
//representing a tree node
void MiniGo::growGameTree(unsigned char level) {
	if (level > 0) {
#pragma omp parallel for schedule(dynamic)//reduction(+:subStatesGenerated)// default(none)		for (unsigned char y = 0; y < primary_node->game_state->dimensions(); y++) {			for (unsigned char x = 0; x < primary_node->game_state->dimensions(); x++) {		//#pragma omp critical
		/*
		 qDebug() << "thread" << omp_get_thread_num()
		 << "x" << x << "y" << y;
		 qDebug() << "level" << level;
		 qDebug() << "generated state" << y * dimensions + x;
		 qDebug() << "at level" << QString::number(level);
		 */
	}
}
}
	//farthest ply ahead; compute heuristic
else
{
}
}

void MiniGo::generateNewLevel(unsigned char levels) {
//unsigned short gssamloops = 0;
if (levels > 1) {
#pragma omp parallel for
for (unsigned short y = 0; y < primary_node->game_state->dimensions(); y++)

	for (unsigned short x = 0; x < primary_node->game_state->dimensions();
			x++) {
	}

//need to figure out if subsequent state indexes should
//correspond to the coordiante at which a stone was
// placed to reach that state
//				mPrimary_state->branch(x, y)->generateNewLevel(levels - 1);

//        for (boost::ptr_vector<State>::iterator iter = subsequentStates.begin();
//        iter not_eq subsequentStates.end(); ++iter)
//        {
//            iter->generateSubsequentStatesAfterMove (level - 1);
//            //gssamloops++;
//        }
} else {
growGameTree(levels);
//        for (boost::ptr_vector<State>::iterator iter
//= subsequentStates.begin();
//       iter not_eq subsequentStates.end(); ++iter)
//       {
//       }
}
//cout << "gssamloops" << gssamloops;
}
