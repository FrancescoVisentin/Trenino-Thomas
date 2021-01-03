#include "Stazione.h"

int main() {
	Treno treno1;
	Stazione_locale st("Padova", 100);
	int a1 = st.handle_new_train(true, treno1);
	int a2 = st.handle_new_train(true, treno1);
	int a3 = st.handle_new_train(false, treno1);
	int a4 = st.handle_new_train(false, treno1);
	int a5 = st.handle_new_train(false, treno1);
	//st.setFullness(false,treno);
	std::cout << a1 << "\t" << a2 << "\t" << a3 << "\t" << a4 << "\t" << a5;
}
