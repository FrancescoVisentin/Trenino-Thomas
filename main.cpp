#include "Stazione.h"

int main() {
	Treno treno1;
	Treno* ptr = &treno1;
	Stazione_locale st("Padova", 100);
	int a1 = st.handle_new_train(true, ptr);
	int a2 = st.handle_new_train(true, ptr);
	int a3 = st.handle_new_train(false, ptr);
	int a4 = st.handle_new_train(false, ptr);
	int a5 = st.handle_new_train(false, ptr);
	//st.setFullness(false,treno);
	std::cout << a1 << "\t" << a2 << "\t" << a3 << "\t" << a4 << "\t" << a5;
}
