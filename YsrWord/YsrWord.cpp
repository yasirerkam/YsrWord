// YsrkmWord.cpp : Konsol uygulamasý için giriþ noktasýný tanýmlar.
//

#include "stdafx.h"
#include <math.h>
#include <time.h>
#include <sstream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_native_dialog.h>
 
struct Matris2B {
	int x1, y1, x2, y2;
};
struct Nokta3 {
	int x1, y1, x2, y2, x3, y3;
};

void RenkPaletiOlustur(ALLEGRO_BITMAP **bmpRenkler, ALLEGRO_COLOR *renkler, ALLEGRO_DISPLAY *display);
bool ButonaBasildi(Matris2B butonKonumu, ALLEGRO_MOUSE_STATE fareDurumu, ALLEGRO_BITMAP *bmp);
void ButonlariCizdir(Matris2B butonKonumu, ALLEGRO_BITMAP *bmp);
void ButonKonumlariAyarlaCiftSira(Matris2B *matris2B, int butonSayisi, int  butonXBaslangic, int butonYBaslangic, int kenarBoyu, int butonArasiFark);
void ButonKonumlariAyarlaTekSira(Matris2B *matris2B, int butonSayisi, int  butonXBaslangic, int butonYBaslangic, int kenarBoyu, int butonArasiFark);
void CizimRengiDegistir(bool *renkAktif, ALLEGRO_COLOR *renkler);
void SayfaOlustur();
void EkranaYazUstr();
void SecimBolmesiniOlustur();
void EkraniCizdir();
void EkrandaCzmKnmnuSfrla();
void UstrImlecHaraketEttir(ALLEGRO_FONT *font, ALLEGRO_USTR *ustr, int yon);
void Sec(ALLEGRO_FONT *font, ALLEGRO_USTR *ustr, int yon);
void Kes(ALLEGRO_USTR **ustr, ALLEGRO_FONT *font);
void TamponaKopyala(ALLEGRO_USTR *ustr);
void TampondanYapistir(ALLEGRO_FONT *font, ALLEGRO_USTR *ustr);
void Kaydet();
bool Yukle();

const int renkSayisi = 8, cizimKalinligiSayisi = 6, butonRenkSayisi = 4, butonCikisGrubuSayisi = 4;
bool kapat, yeniSayfa, kaydet, yukle;
bool abcAktif, ArkaPlanAktif, SecimVeSayfaAktif;
bool renkleriDegistir = false;
bool renkAktif[renkSayisi];
int SCREEN_W, SCREEN_H;
int CaviarDreamsByklgu1 = 16, LinLibertine_aS_Byklgu1 = 140, CollegedByklgu1 = 50;
int imlecPos, imlecSecPos, satirUzunlugu = 60;
int yazimBslngcNktsiX, yazimBslngcNktsiY;
int cizimKalinligi;
int cizimKalinliklari[cizimKalinligiSayisi];
int secimBolgesi;
int sayfaSayisi = 1;
const float FPS = 60;
float cizgi_dx = -4.0, cizgi_dy = 4.0;
Matris2B konumButonlarRenk[butonRenkSayisi];
Matris2B konumButonlarCikisGurubu[butonCikisGrubuSayisi];
Matris2B konumButonlarRenkler[renkSayisi];
Matris2B konumButonlarRenkKalinliklari[cizimKalinligiSayisi];
ALLEGRO_DISPLAY *display;
ALLEGRO_TRANSFORM kamera;
ALLEGRO_USTR *ustrTampon, *ustr;
ALLEGRO_MOUSE_STATE fareDurumu;
ALLEGRO_KEYBOARD_STATE klavyeDurumu;
ALLEGRO_EVENT_QUEUE *event_queue;
ALLEGRO_TIMER *zamanlayici;
ALLEGRO_FONT *CaviarDreams, *Colleged, *LinLibertine_aS;
ALLEGRO_BITMAP *butonCikis, *butonKaydet, *butonYukle, *butonYeniSayfa, *butonRenkleriDegistir, *butonAbc, *butonArkaPlan, *butonSecimVeSayfa, *bmpSecimVeSayfaRengi, *bmpRenkler[renkSayisi], *bmpCizimKalinliklari[cizimKalinligiSayisi], *bmpResim;;
ALLEGRO_COLOR renkler[renkSayisi], cizimRengi, yaziRengi, arkaPlanRengi, secimVeSayfaRengi;
ALLEGRO_FILE *file;

int main()
{
	if (!al_init()) {
		printf("allegro yüklenemedi!\n");
		return -1;
	}
	if (!al_init_image_addon()) {
		printf("allegro image yüklenemedi!\n");
		return -1;
	}
	if (!al_init_primitives_addon()) {
		printf("allegro primitives yüklenemedi!\n");
		return -1;
	}
	if (!al_init_ttf_addon()) {
		printf("allegro ttf yüklenemedi!\n");
		return -1;
	}
	if (!al_init_font_addon()) {
		printf("allegro font yüklenemedi!\n");
		return -1;
	}
	if (!al_init_native_dialog_addon()) {
		printf("allegro dialog yüklenemedi!\n");
		return -1;
	}

	ALLEGRO_DISPLAY_MODE   disp_data;
	al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	SCREEN_W = disp_data.width;
	SCREEN_H = disp_data.height;

	float kameraPos[2] = { 0, 0 };
	secimBolgesi = SCREEN_W - 100;
	ustr = al_ustr_new("");

	butonCikis = al_load_bitmap("Sprites//ButonCikisGrubu//cikis.png");
	butonKaydet = al_load_bitmap("Sprites//ButonCikisGrubu//kaydet.png");
	butonYukle = al_load_bitmap("Sprites//ButonCikisGrubu//yukle.png");
	butonYeniSayfa = al_load_bitmap("Sprites//ButonCikisGrubu//yeniSayfa.png");
	butonRenkleriDegistir = al_load_bitmap("Sprites//ButonRenk//gokkusagi.png");
	butonAbc = al_load_bitmap("Sprites//ButonRenk//abc.png");
	butonArkaPlan = al_load_bitmap("Sprites//ButonRenk//arkaPlan.png");
	butonSecimVeSayfa = al_load_bitmap("Sprites//ButonRenk//secimVeSayfa.png");
	bmpRenkler[renkSayisi];
	bmpCizimKalinliklari[cizimKalinligiSayisi];
	bmpResim = al_create_bitmap(secimBolgesi, SCREEN_H);

	kapat = yeniSayfa = kaydet = yukle = false;
	abcAktif = ArkaPlanAktif = SecimVeSayfaAktif = false;

	CaviarDreams = al_load_ttf_font("CaviarDreams.ttf", CaviarDreamsByklgu1, NULL);
	Colleged = al_load_ttf_font("Colleged.ttf", CollegedByklgu1, NULL);
	LinLibertine_aS = al_load_ttf_font("LinLibertine_aS.ttf", LinLibertine_aS_Byklgu1, 0);
	if (!Colleged || !LinLibertine_aS || !CaviarDreams) {
		printf("font yüklenemedi!\n");
		al_destroy_display(display);
		al_destroy_timer(zamanlayici);
		al_rest(3);
		return -1;
	}

	display = al_create_display(SCREEN_W, SCREEN_H);
	if (!display) {
		printf("ekran oluþturulamadý!\n");
		al_destroy_timer(zamanlayici);
		return -1;
	}

	zamanlayici = al_create_timer(1.0 / FPS);
	if (!zamanlayici) {
		printf("zamanlayýcý oluþturulamadi!\n");
		return -1;
	}

	event_queue = al_create_event_queue();
	if (!event_queue) {
		printf("olay kuyruðu oluþturulamadý!\n");
		al_destroy_display(display);
		al_destroy_timer(zamanlayici);
		return -1;
	}

	if (!al_install_mouse()) {
		printf("mause yüklenemedi!\n");
		al_destroy_display(display);
		al_destroy_timer(zamanlayici);
		return -1;
	}

	if (!al_install_keyboard()) {
		printf("klavye yüklenemedi!\n");
		al_destroy_display(display);
		al_destroy_timer(zamanlayici);
		return -1;
	}

	// renkler atanýyor
	RenkPaletiOlustur(bmpRenkler, renkler, display);
	// renkler atanýyor bitiþ
	// renklerin ilk aktiflik durumu atanýyor
	renkAktif[0] = true;
	for (int i = 1; i < renkSayisi; ++i) {
		renkAktif[i] = false;
	}
	// renklerin ilk aktiflik durumu atanýyor bitiþ
	// cizgiKalinliklari spritelari atanýyor
	for (int i = 0; i < cizimKalinligiSayisi; ++i) {
		std::stringstream str;
		str << "Sprites//bmpCizimKalinligi//bmpCizimKalinligi" << i + 1 << ".png";
		bmpCizimKalinliklari[i] = al_load_bitmap(str.str().c_str());
		if (!bmpCizimKalinliklari[i]) {
			printf("bmpCizimKalinligi yüklenemedi!\n");
			al_rest(3);
			return -1;
		}
	}
	// cizgiKalinliklari spritelari atanýyor bitiþ
	// cizgiKalinliklari atanýyor
	cizimKalinliklari[0] = 14;
	for (int i = 1; i < cizimKalinligiSayisi; ++i) {
		cizimKalinliklari[i] = cizimKalinliklari[i - 1] + 4;
		if (!bmpCizimKalinliklari[i]) {
			printf("cizgikalinliklari atanamadi!\n");
			al_rest(3);
			return -1;
		}
	}
	// cizgiKalinliklari atanýyor bitiþ
	// renklerin ilk aktiflik durumu atanýyor bitiþ

	yazimBslngcNktsiX = (SCREEN_W / 2 - satirUzunlugu * 9 / 2);
	yazimBslngcNktsiY = 100;
	yaziRengi = renkler[0];
	arkaPlanRengi = renkler[3];

	secimVeSayfaRengi = renkler[4];
	bmpSecimVeSayfaRengi = al_create_bitmap(25, 25);
	al_set_target_bitmap(bmpSecimVeSayfaRengi);
	al_clear_to_color(secimVeSayfaRengi);
	al_set_target_bitmap(al_get_backbuffer(display));

	// Ýlk Ekran Görüntüleri
	al_clear_to_color(al_map_rgb(255, 255, 255));
	al_draw_text(LinLibertine_aS, al_map_rgb(0, 0, 0), SCREEN_W / 2, SCREEN_H / 2 - LinLibertine_aS_Byklgu1 / 2, ALLEGRO_ALIGN_CENTER, "YSR WORD");
	al_flip_display();
	al_rest(1.25);
	al_clear_to_color(al_map_rgb(250, 200, 20));
	al_draw_text(Colleged, al_map_rgb(0, 120, 250), SCREEN_W / 2, SCREEN_H / 2 - CollegedByklgu1 / 2, ALLEGRO_ALIGN_CENTRE, "Yasir Erkam Ozdemir");
	al_flip_display();
	al_rest(2);
	// Ýlk Ekran Görüntüleri bitiþ

	// --butonlarýn konumlari ayarlanýyor
	ButonKonumlariAyarlaCiftSira(konumButonlarRenk, butonRenkSayisi, secimBolgesi + 23, 180, 25, 5);
	ButonKonumlariAyarlaCiftSira(konumButonlarRenkler, renkSayisi, secimBolgesi + 23, 245, 25, 5);
	ButonKonumlariAyarlaTekSira(konumButonlarCikisGurubu, butonCikisGrubuSayisi, secimBolgesi + 25, 390, 50, 10);
	// --butonlarýn konumlari ayarlanýyor bitiþ

	EkraniCizdir();

	// olay örgüsü atamalarý
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(zamanlayici));
	al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	// olay örgüsü atamalarý bitiþ

	int ucgenKoseSayisi;
	// Döngüye giriliyor
	al_start_timer(zamanlayici);
	while (true)
	{
		ALLEGRO_EVENT events;
		al_wait_for_event(event_queue, &events);

		al_get_mouse_state(&fareDurumu);
		al_get_keyboard_state(&klavyeDurumu);

		if (events.type == ALLEGRO_EVENT_KEY_CHAR)
		{
			if ((al_key_down(&klavyeDurumu, ALLEGRO_KEY_ESCAPE)))	break;

			if (!al_key_down(&klavyeDurumu, ALLEGRO_KEY_LCTRL)) {

				if (events.keyboard.unichar >= 32 && events.keyboard.unichar <= 126)
				{
					//if (al_key_down(&klavyeDurumu, events.keyboard.keycode))
					{
						al_ustr_insert_chr(ustr, imlecPos, events.keyboard.unichar);
						UstrImlecHaraketEttir(CaviarDreams, ustr, ALLEGRO_KEY_RIGHT);//1 artýrmak için ALLEGRO_KEY_RIGHT
						EkraniCizdir();
					}
				}
				if (events.keyboard.unichar == 13)
				{
					al_ustr_insert_chr(ustr, imlecPos, events.keyboard.unichar);
					UstrImlecHaraketEttir(CaviarDreams, ustr, ALLEGRO_KEY_RIGHT);//1 artýrmak için ALLEGRO_KEY_RIGHT
					EkraniCizdir();

				}

				switch (events.keyboard.keycode)
				{
				case ALLEGRO_KEY_BACKSPACE:
					al_ustr_remove_range(ustr, imlecPos - 1, imlecPos);
					UstrImlecHaraketEttir(CaviarDreams, ustr, ALLEGRO_KEY_LEFT);//1 azaltmak için ALLEGRO_KEY_LEFT
					EkraniCizdir();
					break;
				case ALLEGRO_KEY_SPACE:
					al_ustr_insert(ustr, imlecPos, al_ustr_new(" "));
					UstrImlecHaraketEttir(CaviarDreams, ustr, ALLEGRO_KEY_RIGHT);//1 artýrmak için ALLEGRO_KEY_RIGHT
					EkraniCizdir();
					break;
				case ALLEGRO_KEY_UP:
					UstrImlecHaraketEttir(CaviarDreams, ustr, ALLEGRO_KEY_UP);
					break;
				case ALLEGRO_KEY_DOWN:
					UstrImlecHaraketEttir(CaviarDreams, ustr, ALLEGRO_KEY_DOWN);
					break;
				case ALLEGRO_KEY_LEFT:
					UstrImlecHaraketEttir(CaviarDreams, ustr, ALLEGRO_KEY_LEFT);
					break;
				case ALLEGRO_KEY_RIGHT:
					UstrImlecHaraketEttir(CaviarDreams, ustr, ALLEGRO_KEY_RIGHT);
					break;
				}
			}
			else
			{

				switch (events.keyboard.keycode)
				{
				case ALLEGRO_KEY_LEFT:
					Sec(CaviarDreams, ustr, ALLEGRO_KEY_LEFT);
					break;
				case ALLEGRO_KEY_RIGHT:
					Sec(CaviarDreams, ustr, ALLEGRO_KEY_RIGHT);
					break;
				case ALLEGRO_KEY_X:
					Kes(&ustr, CaviarDreams);
					break;
				case ALLEGRO_KEY_C:
					TamponaKopyala(ustr);
					break;
				case ALLEGRO_KEY_V:
					TampondanYapistir(CaviarDreams, ustr);
					break;
				}
			}

		}

		if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {

			if (ButonaBasildi(konumButonlarCikisGurubu[3], fareDurumu, butonCikis)) break;
			else if (ButonaBasildi(konumButonlarCikisGurubu[2], fareDurumu, butonKaydet)) {
				Kaydet();
			}
			else if (ButonaBasildi(konumButonlarCikisGurubu[1], fareDurumu, butonYukle)) {
				Yukle();
			}
			else if (ButonaBasildi(konumButonlarCikisGurubu[0], fareDurumu, butonYeniSayfa)) {
				al_ustr_free(ustr);
				ustr = al_ustr_new("");
				imlecPos = 0;
			}

			else if (ButonaBasildi(konumButonlarRenk[0], fareDurumu, butonRenkleriDegistir)) {
				RenkPaletiOlustur(bmpRenkler, renkler, display);
			}
			else if (ButonaBasildi(konumButonlarRenk[1], fareDurumu, butonAbc)) {
				if (abcAktif == false) {
					abcAktif = true;
					ArkaPlanAktif = false;
					SecimVeSayfaAktif = false;
				}
			}
			else if (ButonaBasildi(konumButonlarRenk[2], fareDurumu, butonArkaPlan)) {
				if (ArkaPlanAktif == false) {
					abcAktif = false;
					ArkaPlanAktif = true;
					SecimVeSayfaAktif = false;
				}
			}
			else if (ButonaBasildi(konumButonlarRenk[3], fareDurumu, butonSecimVeSayfa)) {
				if (SecimVeSayfaAktif == false) {
					abcAktif = false;
					ArkaPlanAktif = false;
					SecimVeSayfaAktif = true;
				}
			}

			if (abcAktif) {
				for (int i = 0; i < renkSayisi; ++i)
				{
					if (ButonaBasildi(konumButonlarRenkler[i], fareDurumu, bmpRenkler[i]))
					{
						yaziRengi = renkler[i];
					}
				}
			}
			else if (ArkaPlanAktif) {
				for (int i = 0; i < renkSayisi; ++i)
				{
					if (ButonaBasildi(konumButonlarRenkler[i], fareDurumu, bmpRenkler[i]))
					{
						arkaPlanRengi = renkler[i];
					}
				}
			}
			else if (SecimVeSayfaAktif) {
				for (int i = 0; i < renkSayisi; ++i)
				{
					if (ButonaBasildi(konumButonlarRenkler[i], fareDurumu, bmpRenkler[i]))
					{
						secimVeSayfaRengi = renkler[i];
						al_set_target_bitmap(bmpSecimVeSayfaRengi);
						al_clear_to_color(secimVeSayfaRengi);
						al_set_target_bitmap(al_get_backbuffer(display));
					}
				}
			}

			for (int i = 0; i < renkSayisi; ++i)
			{
				if (ButonaBasildi(konumButonlarRenkler[i], fareDurumu, bmpRenkler[i]))
				{
					if (renkAktif[i])
						renkAktif[i] = false;
					else
					{
						for (int j = 0; j < renkSayisi; ++j) {
							if (i == j)
								renkAktif[j] = true;
							else
								renkAktif[j] = false;
						}
					}
				}
			}

		}

		if (events.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
			EkraniCizdir();
		}

		if (events.type == ALLEGRO_EVENT_MOUSE_AXES) {
			EkraniCizdir();
		}

		if (events.type == ALLEGRO_EVENT_TIMER)
		{
			CizimRengiDegistir(renkAktif, renkler);
		}

	}
	///Kaydet();

	al_destroy_font(CaviarDreams);
	al_destroy_timer(zamanlayici);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	return 0;
}

void Sec(ALLEGRO_FONT *font, ALLEGRO_USTR *ustr, int yon) {
	int yazininUzunlugu = al_ustr_size(ustr);

	switch (yon) {
		//case ALLEGRO_KEY_UP:
		//	imlecSecPos -= satirUzunlugu;
		//	break;
		//case ALLEGRO_KEY_DOWN:
		//	imlecSecPos += satirUzunlugu;
		//	break;
	case ALLEGRO_KEY_LEFT:
		imlecSecPos -= 1;
		break;
	case ALLEGRO_KEY_RIGHT:
		imlecSecPos += 1;
		break;
	}
	if (imlecSecPos < 0) {
		imlecSecPos = 0;
	}
	else if (imlecSecPos > yazininUzunlugu) {
		imlecSecPos = yazininUzunlugu;
	}

	EkraniCizdir();
}

void Kes(ALLEGRO_USTR **ustr, ALLEGRO_FONT *font)
{
	if (imlecPos < imlecSecPos) {
		ustrTampon = al_ustr_dup_substr(*ustr, imlecPos, imlecSecPos);
		al_ustr_remove_range(*ustr, imlecPos, imlecSecPos);
	}
	else if (imlecPos > imlecSecPos) {
		ustrTampon = al_ustr_dup_substr(*ustr, imlecSecPos, imlecPos);
		al_ustr_remove_range(*ustr, imlecSecPos, imlecPos);
		imlecPos -= al_ustr_size(ustrTampon);
	}

	EkraniCizdir();
}

void TamponaKopyala(ALLEGRO_USTR *ustr)
{
	if (imlecPos < imlecSecPos) {
		ustrTampon = al_ustr_dup_substr(ustr, imlecPos, imlecSecPos);
	}
	else if (imlecPos > imlecSecPos) {
		ustrTampon = al_ustr_dup_substr(ustr, imlecSecPos, imlecPos);
	}
}

void TampondanYapistir(ALLEGRO_FONT *font, ALLEGRO_USTR *ustr)
{
	al_ustr_insert(ustr, imlecPos, ustrTampon);
	imlecPos += al_ustr_size(ustrTampon);
	EkraniCizdir();
}

void UstrImlecHaraketEttir(ALLEGRO_FONT *font, ALLEGRO_USTR *ustr, int yon)
{
	int yazininUzunlugu = al_ustr_size(ustr);

	switch (yon) {
	case ALLEGRO_KEY_UP:
		imlecPos -= satirUzunlugu;
		break;
	case ALLEGRO_KEY_DOWN:
		imlecPos += satirUzunlugu;
		break;
	case ALLEGRO_KEY_LEFT:
		imlecPos -= 1;
		break;
	case ALLEGRO_KEY_RIGHT:
		imlecPos += 1;
		break;
	}
	if (imlecPos < 0) {
		imlecPos = 0;
	}
	else if (imlecPos > yazininUzunlugu) {
		imlecPos = yazininUzunlugu;
	}

	imlecSecPos = imlecPos;

	EkraniCizdir();
}

void Kaydet()
{
	file = al_fopen("yazi.txt", "wb");
	al_fputs(file, al_cstr(ustr));
	al_fclose(file);
}

bool Yukle()
{
	file = al_fopen("yazi.txt", "rb");
	if (file == NULL)
	{
		al_fclose(file);
		return false;
	}

	al_ustr_free(ustr);
	ustr = al_fget_ustr(file);
	if (ustr == NULL)
	{
		ustr = al_ustr_new("");
		imlecPos = al_ustr_size(ustr);
		al_fclose(file);
		return false;
	}
	else {
		imlecPos = al_ustr_size(ustr);
		al_fclose(file);
		return true;
	}
}

void EkrandaCzmKnmnuDgstr()
{
	al_get_mouse_state(&fareDurumu);

	ALLEGRO_TRANSFORM t;
	al_identity_transform(&t);
	al_translate_transform(&t, 0, 20 * al_get_mouse_state_axis(&fareDurumu, 2));
	al_use_transform(&t);
}
void EkrandaCzmKnmnuSfrla()
{
	ALLEGRO_TRANSFORM t;
	al_identity_transform(&t);
	al_translate_transform(&t, 0, 0);
	al_use_transform(&t);
}

void EkraniCizdir()
{
	SayfaOlustur();
	SecimBolmesiniOlustur();
	EkranaYazUstr();

	al_flip_display();
}

void SayfaOlustur()
{

	EkrandaCzmKnmnuDgstr();

	al_clear_to_color(arkaPlanRengi);
	int sayfaBoyutuX = 750, sayfaBoyutuY = 900;
	for (int i = 0; i < sayfaSayisi; ++i) {
		al_draw_scaled_bitmap(bmpSecimVeSayfaRengi, 0, 0, al_get_bitmap_width(bmpSecimVeSayfaRengi), al_get_bitmap_height(bmpSecimVeSayfaRengi), SCREEN_W / 2 - sayfaBoyutuX / 2, 20 + i*(sayfaBoyutuY + 20), sayfaBoyutuX, sayfaBoyutuY, NULL);
	}

	EkrandaCzmKnmnuSfrla();
}

void EkranaYazUstr()
{
	ALLEGRO_USTR *ustrImlecli = al_ustr_dup(ustr);
	al_ustr_insert_cstr(ustrImlecli, imlecPos, "|");
	int yazininUzunlugu = al_ustr_size(ustrImlecli);
	//	al_ustr_insert_cstr(ustrImlecli, imlecSecPos, "*");

	EkrandaCzmKnmnuDgstr();

	int j = 0;
	int sayac = 0;
	sayfaSayisi = 1;

	for (int i = 0; i <= (yazininUzunlugu); ++i)
	{
		char enter = 13;

		ALLEGRO_USTR *ustrGecici = al_ustr_dup_substr(ustrImlecli, i, i + 1);

		if (i != al_ustr_find_chr(ustrImlecli, i, 13))
		{
			ALLEGRO_USTR *ustrGecici1 = al_ustr_dup_substr(ustrImlecli, i - sayac, i + 1);
			al_draw_ustr(CaviarDreams, yaziRengi, yazimBslngcNktsiX, yazimBslngcNktsiY + j * (CaviarDreamsByklgu1 + 1), ALLEGRO_ALIGN_LEFT, ustrGecici1);
			++sayac;
		}
		else {
			sayac = 0;
			++j;
		}

		if (sayac > satirUzunlugu) {
			sayac = 0;
			++j;
		}

		if (j == 45 || j == 100 || j == 155) {
			sayfaSayisi++;
			j += 10;
		}

	}

	EkrandaCzmKnmnuSfrla();
}

//------------------------
void RenkPaletiOlustur(ALLEGRO_BITMAP **bmpRenkler, ALLEGRO_COLOR *renkler, ALLEGRO_DISPLAY *display)
{
	srand(time(NULL));
	//ilk iki renk için siyah ve beyaz atanýyor
	bmpRenkler[0] = al_create_bitmap(25, 25);
	al_set_target_bitmap(bmpRenkler[0]);
	renkler[0] = al_map_rgb(0, 0, 0);
	al_clear_to_color(renkler[0]);
	//
	bmpRenkler[1] = al_create_bitmap(25, 25);
	al_set_target_bitmap(bmpRenkler[1]);
	renkler[1] = al_map_rgb(255, 255, 255);
	al_clear_to_color(renkler[1]);
	//sonraki renkler ataniyor
	for (int i = 2; i < renkSayisi; ++i) {
		bmpRenkler[i] = al_create_bitmap(25, 25);
		al_set_target_bitmap(bmpRenkler[i]);
		renkler[i] = al_map_rgb(rand() % 256, rand() % 256, rand() % 256);
		al_clear_to_color(renkler[i]);
	}
	al_set_target_bitmap(al_get_backbuffer(display));
}

void CizimRengiDegistir(bool *renkAktif, ALLEGRO_COLOR *renkler)
{
	for (int i = 0; i < renkSayisi; ++i) {
		if (renkAktif[i])
			cizimRengi = renkler[i];
	}
}

bool ButonaBasildi(Matris2B butonKonumu, ALLEGRO_MOUSE_STATE fareDurumu, ALLEGRO_BITMAP *bmp)
{
	if (fareDurumu.x > butonKonumu.x1 && fareDurumu.x < butonKonumu.x2 && fareDurumu.y < butonKonumu.y2 && fareDurumu.y>butonKonumu.y1 && (al_mouse_button_down(&fareDurumu, 1)))
	{
		al_draw_filled_rectangle(butonKonumu.x1, butonKonumu.y1, butonKonumu.x2, butonKonumu.y2, al_map_rgb(250, 200, 100)); //buton iç rengi
		al_draw_filled_rectangle(butonKonumu.x1 - 1, butonKonumu.y1, butonKonumu.x1, butonKonumu.y2, al_map_rgb(0, 0, 0));
		al_draw_filled_rectangle(butonKonumu.x1 - 1, butonKonumu.y1 - 1, butonKonumu.x2, butonKonumu.y1, al_map_rgb(0, 0, 0));
		al_draw_filled_rectangle(butonKonumu.x1 - 1, butonKonumu.y2, butonKonumu.x2, butonKonumu.y2 + 1, al_map_rgb(255, 255, 255));
		al_draw_filled_rectangle(butonKonumu.x2, butonKonumu.y1, butonKonumu.x2 + 1, butonKonumu.y2, al_map_rgb(255, 255, 255));
		al_draw_scaled_bitmap(bmp, 0, 0, al_get_bitmap_width(bmp), al_get_bitmap_height(bmp), butonKonumu.x1 + 5, butonKonumu.y1 + 5, butonKonumu.x2 - butonKonumu.x1 - 10, butonKonumu.y2 - butonKonumu.y1 - 10, NULL);
		al_flip_display();
		return true;
	}
	else
	{
		/*al_draw_filled_rectangle(butonKonumu.x1 - 2, butonKonumu.y1 - 2, butonKonumu.x2 + 2, butonKonumu.y2 + 2, al_map_rgb(0, 0, 0));
		al_draw_filled_rectangle(butonKonumu.x1, butonKonumu.y1, butonKonumu.x2, butonKonumu.y2, al_map_rgb(255, 255, 255)); //buton iç rengi
		al_draw_filled_rectangle(butonKonumu.x1 - 1, butonKonumu.y1, butonKonumu.x1, butonKonumu.y2, al_map_rgb(255, 255, 255));
		al_draw_filled_rectangle(butonKonumu.x1 - 1, butonKonumu.y1 - 1, butonKonumu.x2, butonKonumu.y1, al_map_rgb(255, 255, 255));
		al_draw_scaled_bitmap(bmp, 0, 0, al_get_bitmap_width(bmp), al_get_bitmap_height(bmp), butonKonumu.x1 + 2, butonKonumu.y1 + 2, butonKonumu.x2 - butonKonumu.x1 - 4, butonKonumu.y2 - butonKonumu.y1 - 4, NULL);
		al_flip_display();*/
		return false;
	}
}
void SecimBolmesiniOlustur()
{
	al_draw_scaled_bitmap(bmpSecimVeSayfaRengi, 0, 0, al_get_bitmap_width(bmpSecimVeSayfaRengi), al_get_bitmap_height(bmpSecimVeSayfaRengi), secimBolgesi, 0, SCREEN_W - secimBolgesi, SCREEN_H, NULL);

	//butonlar cizdiriliyor
	ButonlariCizdir(konumButonlarCikisGurubu[3], butonCikis);
	ButonlariCizdir(konumButonlarCikisGurubu[2], butonKaydet);
	ButonlariCizdir(konumButonlarCikisGurubu[1], butonYukle);
	ButonlariCizdir(konumButonlarCikisGurubu[0], butonYeniSayfa);
	ButonlariCizdir(konumButonlarRenk[0], butonRenkleriDegistir);
	ButonlariCizdir(konumButonlarRenk[1], butonAbc);
	ButonlariCizdir(konumButonlarRenk[2], butonArkaPlan);
	ButonlariCizdir(konumButonlarRenk[3], butonSecimVeSayfa);
	for (int i = 0; i < renkSayisi; ++i) {
		ButonlariCizdir(konumButonlarRenkler[i], bmpRenkler[i]);
	}
}
void ButonlariCizdir(Matris2B butonKonumu, ALLEGRO_BITMAP *bmp)
{
	al_draw_filled_rectangle(butonKonumu.x1 - 2, butonKonumu.y1 - 2, butonKonumu.x2 + 2, butonKonumu.y2 + 2, al_map_rgb(0, 0, 0));
	al_draw_filled_rectangle(butonKonumu.x1, butonKonumu.y1, butonKonumu.x2, butonKonumu.y2, al_map_rgb(255, 255, 255)); //buton iç rengi
	al_draw_filled_rectangle(butonKonumu.x1 - 1, butonKonumu.y1, butonKonumu.x1, butonKonumu.y2, al_map_rgb(255, 255, 255));
	al_draw_filled_rectangle(butonKonumu.x1 - 1, butonKonumu.y1 - 1, butonKonumu.x2, butonKonumu.y1, al_map_rgb(255, 255, 255));
	al_draw_scaled_bitmap(bmp, 0, 0, al_get_bitmap_width(bmp), al_get_bitmap_height(bmp), butonKonumu.x1 + 2, butonKonumu.y1 + 2, butonKonumu.x2 - butonKonumu.x1 - 4, butonKonumu.y2 - butonKonumu.y1 - 4, NULL);
}

void ButonKonumlariAyarlaCiftSira(Matris2B *matris2B, int butonSayisi, int  butonXBaslangic, int butonYBaslangic, int kenarBoyu, int butonArasiFark)
{
	matris2B[0].y1 = butonYBaslangic;
	matris2B[0].y2 = butonYBaslangic + kenarBoyu;
	for (int i = 0; i < butonSayisi; i += 2) {
		matris2B[i].x1 = butonXBaslangic;
		matris2B[i].x2 = butonXBaslangic + kenarBoyu;
	}
	for (int i = 2; i < butonSayisi; i += 2) {
		matris2B[i].y1 = matris2B[i - 2].y1 + kenarBoyu + butonArasiFark;
		matris2B[i].y2 = matris2B[i - 2].y2 + kenarBoyu + butonArasiFark;
	}
	matris2B[1].y1 = butonYBaslangic;
	matris2B[1].y2 = butonYBaslangic + kenarBoyu;
	for (int i = 1; i < butonSayisi; i += 2) {
		matris2B[i].x1 = butonXBaslangic + kenarBoyu + butonArasiFark;
		matris2B[i].x2 = butonXBaslangic + 2 * kenarBoyu + butonArasiFark;
	}
	for (int i = 3; i < butonSayisi; i += 2) {
		matris2B[i].y1 = matris2B[i - 2].y1 + kenarBoyu + butonArasiFark;
		matris2B[i].y2 = matris2B[i - 2].y2 + kenarBoyu + butonArasiFark;
	}
}
void ButonKonumlariAyarlaTekSira(Matris2B *matris2B, int butonSayisi, int  butonXBaslangic, int butonYBaslangic, int kenarBoyu, int butonArasiFark)
{
	matris2B[0].y1 = butonYBaslangic;
	matris2B[0].y2 = butonYBaslangic + kenarBoyu;
	for (int i = 0; i < butonSayisi; ++i) {
		matris2B[i].x1 = butonXBaslangic;
		matris2B[i].x2 = butonXBaslangic + kenarBoyu;
	}
	for (int i = 1; i < butonSayisi; ++i) {
		matris2B[i].y1 = matris2B[i - 1].y1 + kenarBoyu + butonArasiFark;
		matris2B[i].y2 = matris2B[i - 1].y2 + kenarBoyu + butonArasiFark;
	}
}