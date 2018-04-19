#pragma once
#include "j_plot.hpp"

/*
Determine traces to plot from the control part of the main circuit
*/
void traces_to_plot(std::vector<std::string> controlPart, std::vector<std::string>& traceLabel, std::vector<std::vector<double>>& traceData) {
	std::vector<std::string> tokens;
	std::vector<double> trace;
	std::map<std::string, std::vector<double>> traces;
	std::string columnLabel1, columnLabel2, label;
	int index1 = -1;
	int index2 = -1;
	for (auto string : controlPart) {
		if (string.find(".PRINT") != std::string::npos) {
			tokens = tokenize_space(string);
			/* Print the identified node voltage */
			if (tokens[1] == "NODEV") {
				/* If more than one node is specified */
				if (tokens.size() == 4) {
					/* If second node is ground */
					if(tokens[3] == "0" || tokens[3] == "GND") {
						label = "NODE VOLTAGE " + tokens[2];
						columnLabel1 = "C_NV" + tokens[2];
						if (std::find(columnNames.begin(), columnNames.end(), columnLabel1) != columnNames.end()) {
							index1 = index_of(columnNames, columnLabel1);
							traceLabel.push_back(label);
							traceData.push_back(x[index1]);
						}
						else {
							/* Error this node was not found and can therefore not be printed */
						}
					}
					/* If first node is ground */
					else if (tokens[2] == "0" || tokens[3] == "GND") {
						label = "NODE VOLTAGE " + tokens[3];
						columnLabel1 = "C_NV" + tokens[3];
						if (std::find(columnNames.begin(), columnNames.end(), columnLabel1) != columnNames.end()) {
							index1 = index_of(columnNames, columnLabel1);
							trace.clear();
							trace = x[index1];
							for (int m = 0; m < trace.size(); m++) {
								trace[m] = 0.0;
							}
							std::transform(trace.begin(), trace.end(), x[index2].begin(), trace.begin(), std::minus<double>());
							traceLabel.push_back(label);
							traceData.push_back(trace);
						}
						else {
							/* Error this node was not found and can therefore not be printed */
						}
					}
					/* If neither are ground*/
					else {
						label = "NODE VOLTAGE " + tokens[2] + " to " + tokens[3];
						columnLabel1 = "C_NV" + tokens[2];
						columnLabel2 = "C_NV" + tokens[3];
						if (std::find(columnNames.begin(), columnNames.end(), columnLabel1) != columnNames.end()) {
							index1 = index_of(columnNames, columnLabel1);
							trace.clear();
							trace = x[index1];
							if (std::find(columnNames.begin(), columnNames.end(), columnLabel1) != columnNames.end()) {
								index2 = index_of(columnNames, columnLabel2);
								std::transform(x[index1].begin(), x[index1].end(), x[index2].begin(), trace.begin(), std::minus<double>());
								traceLabel.push_back(label);
								traceData.push_back(trace);
							}
							else {
								/* Error this node was not found and can therefore not be printed */
							}
						}
						else {
							/* Error this node was not found and can therefore not be printed */
						}
					}
				}
				/* If only one node is specified */
				else {
					label = "NODE VOLTAGE " + tokens[2];
					columnLabel1 = "C_NV" + tokens[2];
					if (std::find(columnNames.begin(), columnNames.end(), columnLabel1) != columnNames.end()) {
						index1 = index_of(columnNames, columnLabel1);
						traceLabel.push_back(label);
						traceData.push_back(x[index1]);
					}
					else {
						/* Error this node was not found and can therefore not be printed */
					}
				}
			}
			/* Print the identified junction phase */
			else if (tokens[1] == "PHASE") {
				label = "PHASE " + tokens[2];
				columnLabel1 = "C_P" + tokens[2];
				if (std::find(columnNames.begin(), columnNames.end(), columnLabel1) != columnNames.end()) {
					index1 = index_of(columnNames, columnLabel1);
					traceLabel.push_back(label);
					traceData.push_back(x[index1]);
				}
				else {
					/* Error this node was not found and can therefore not be printed */
				}
			}
			/* Print the identified device voltage */
			else if (tokens[1] == "DEVV") {
				label = "NOTHING";
				for (auto i : elements) {
					if (i.label == tokens[2]) {
						trace.clear();
						if (i.VPindex == -1) trace = x[i.VNindex];
						else if (i.VNindex == -1) trace = x[i.VPindex];
						else {
							trace = x[i.VPindex];
							std::transform(x[i.VPindex].begin(), x[i.VPindex].end(), x[i.VNindex].begin(), trace.begin(), std::minus<double>());
						}
						label = "DEVICE VOLTAGE " + i.label;
						traceLabel.push_back(label);
						traceData.push_back(trace);
					}
				}
				if (label == "NOTHING") {
					plotting_errors(NO_SUCH_DEVICE_FOUND, tokens[2]);
				}
			}
			/* Print the identified device current */
			else if (tokens[1] == "DEVI") {
				label = "NOTHING";
				for (auto i : elements) {
					if (i.label == tokens[2]) {
						if (tokens[2][0] == 'R') {
							std::vector<double> trace;
							if (i.VPindex == -1) trace = x[i.VNindex];
							else if (i.VNindex == -1) trace = x[i.VPindex];
							else std::transform(x[i.VPindex].begin(), x[i.VPindex].end(), x[i.VNindex].begin(), trace.begin(), std::minus<double>());
							std::transform(trace.begin(), trace.end(), trace.begin(), std::bind1st(std::multiplies<double>(), (1/i.value)));
							label = "DEVICE CURRENT " + i.label;
							traceLabel.push_back(label);
							traceData.push_back(trace);
						}
						else if (tokens[2][0] == 'C') {

						}
						else if (tokens[2][0] == 'L') {
							std::vector<double> trace;
							if (i.CURindex == -1) simulation_errors(INDUCTOR_CURRENT_NOT_FOUND, i.label);
							else trace = x[i.CURindex];
							label = "DEVICE CURRENT " + i.label;
							traceLabel.push_back(label);
							traceData.push_back(trace);
						}
						else if (tokens[2][0] == 'I') {
							label = "DEVICE CURRENT " + i.label;
							traceLabel.push_back(label);
							traceData.push_back(sources[i.label]);
						}
						else if (tokens[2][0] == 'V') {
							simulation_errors(CURRENT_THROUGH_VOLTAGE_SOURCE, i.label);
						}
						else if (tokens[2][0] == 'B') {
							std::vector<double> trace;
							trace = junctionCurrents[i.label];
							label = "DEVICE CURRENT " + i.label;
							traceLabel.push_back(label);
							traceData.push_back(trace);
						}
						else plotting_errors(NO_SUCH_DEVICE_FOUND, tokens[2]);
					}
				}
				if (label == "NOTHING") {
					plotting_errors(NO_SUCH_DEVICE_FOUND, tokens[2]);
				}
			}
			/* No such print command error thrown */
			else {
				plotting_errors(NO_SUCH_PLOT_TYPE, tokens[1]);
			}
		}
	}
}
/*
	Function that creates a plotting window with all available traces to plot
*/
int plot_all_traces() {
	Fl_Window * win = new Fl_Window(1240, 768);
	Fl_Scroll * scroll = new Fl_Scroll(0, 0, win->w(), win->h());
	std::vector<Fl_Chart *> Charts;
	std::string label;
	int counter = 0;
	for (auto i : columnNames) {
		label = substring_after(i, "C_");
		Charts.push_back(new Fl_Chart(20, 20 + (counter * (scroll->h() / 3)), scroll->w() - 40, (scroll->h()/3 - 20)));
		Charts[counter]->type(FL_LINE_CHART);
		for (int j = 0; j < x[counter].size(); j++) {
			Charts[counter]->add(x[counter][j]);

		}
		Charts[counter]->color(FL_WHITE);
		Charts[counter]->align(FL_ALIGN_INSIDE|FL_ALIGN_CENTER|FL_ALIGN_TOP);
		Charts[counter]->copy_label(label.c_str());
		counter++;
	}
	win->resizable(scroll);
	win->label(INPUT_FILE.c_str());
	win->show();
	return(Fl::run());
}

/*
	Function that creates a plotting window only for the specified plots in the simulation
*/
int plot_traces(InputFile& iFile) {
	std::vector<std::string> traceLabel;
	std::vector<std::vector<double>> traceData;
	traces_to_plot(iFile.controlPart, traceLabel, traceData);
	Fl_Window * win = new Fl_Window(1240, 768);
	Fl_Scroll * scroll = new Fl_Scroll(0, 0, win->w(), win->h());
	std::vector<Fl_Chart *> Charts;
	for (int i = 0; i < traceLabel.size(); i++) {
		Charts.push_back(new Fl_Chart(20, 20 + (i * (scroll->h() / 3)), scroll->w() - 40, (scroll->h() / 3 - 20)));
		Charts[i]->type(FL_LINE_CHART);
		for (int j = 0; j < traceData[i].size(); j++) {
			Charts[i]->add(traceData[i][j]);
		}
		Charts[i]->color(FL_WHITE);
		Charts[i]->align(FL_ALIGN_INSIDE | FL_ALIGN_CENTER | FL_ALIGN_TOP);
		Charts[i]->copy_label(traceLabel[i].c_str());
	}
	win->resizable(win);
	win->label(INPUT_FILE.c_str());
	win->show();
	return(Fl::run());
}