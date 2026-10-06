/* Kelompok 5 - Sistem Seleksi Beasiswa */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ###################################################################### */
/* ##  MAHASISWA 1 : DATA MAHASISWA & INPUT                             ## */
/* ##  Input NRP, nama, semester, IPK, data pendukung                   ## */
/* ##  Validasi IPK & semester, memakai do-while                        ## */
/* ###################################################################### */

/* ===================== KONSTANTA ===================== */
#define MAX_NRP        20
#define MAX_NAMA       60
#define MAX_BUFFER     128

#define SEMESTER_MIN   1
#define SEMESTER_MAX   8      /* D4 = 8 semester, ubah kalau perlu */
#define IPK_MIN        0.0
#define IPK_MAX        4.0
#define TANGGUNGAN_MAX 15
#define PRESTASI_MAX   50

/* ===================== STRUCT DATA ===================== */
typedef struct {
    char  nrp[MAX_NRP];
    char  nama[MAX_NAMA];
    int   semester;
    float ipk;
    long  penghasilanOrtu;     /* per bulan, dalam Rupiah */
    int   jumlahTanggungan;    /* jumlah tanggungan orang tua */
    int   jumlahPrestasi;      /* jumlah prestasi/sertifikat */
} Mahasiswa;

/* ===================== FUNGSI BANTU INPUT ===================== */

/* Baca satu baris dengan aman, hapus karakter newline di akhir */
static void bacaBaris(const char *prompt, char *buf, int ukuran)
{
    printf("%s", prompt);
    if (fgets(buf, ukuran, stdin) == NULL) {
        buf[0] = '\0';
        return;
    }
    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') {
        buf[len - 1] = '\0';
    } else {
        /* input kepanjangan: buang sisa karakter di buffer */
        int c;
        while ((c = getchar()) != '\n' && c != EOF) { }
    }
}

/* Hapus spasi di awal dan akhir string */
static void trim(char *s)
{
    char *awal = s;
    while (*awal && isspace((unsigned char)*awal)) awal++;
    memmove(s, awal, strlen(awal) + 1);

    size_t len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) {
        s[--len] = '\0';
    }
}

/* Baca bilangan bulat; return 1 kalau valid, 0 kalau bukan angka */
static int bacaInt(const char *prompt, long *hasil)
{
    char buf[MAX_BUFFER];
    char *akhir;

    bacaBaris(prompt, buf, sizeof(buf));
    trim(buf);
    if (buf[0] == '\0') return 0;

    *hasil = strtol(buf, &akhir, 10);
    return (*akhir == '\0');
}

/* Baca bilangan desimal; koma (3,75) otomatis dianggap titik (3.75) */
static int bacaFloat(const char *prompt, float *hasil)
{
    char buf[MAX_BUFFER];
    char *akhir;

    bacaBaris(prompt, buf, sizeof(buf));
    trim(buf);
    if (buf[0] == '\0') return 0;

    for (char *p = buf; *p; p++) {
        if (*p == ',') *p = '.';
    }

    *hasil = (float)strtod(buf, &akhir);
    return (*akhir == '\0');
}

/* ===================== FUNGSI VALIDASI ===================== */

static int nrpValid(const char *nrp)
{
    size_t len = strlen(nrp);
    if (len == 0) return 0;
    for (size_t i = 0; i < len; i++) {
        if (!isdigit((unsigned char)nrp[i])) return 0;
    }
    return 1;
}

static int namaValid(const char *nama)
{
    size_t len = strlen(nama);
    if (len == 0) return 0;
    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)nama[i];
        if (!(isalpha(c) || c == ' ' || c == '.' || c == '\'' || c == '-'))
            return 0;
    }
    return 1;
}

/* ===================== FUNGSI INPUT (do-while) ===================== */

static void inputNRP(Mahasiswa *m)
{
    int valid;
    do {
        bacaBaris("NRP                      : ", m->nrp, MAX_NRP);
        trim(m->nrp);
        valid = nrpValid(m->nrp);
        if (!valid) {
            printf("  [!] NRP tidak valid. Harus berupa angka saja dan tidak kosong.\n");
        }
    } while (!valid);
}

static void inputNama(Mahasiswa *m)
{
    int valid;
    do {
        bacaBaris("Nama                     : ", m->nama, MAX_NAMA);
        trim(m->nama);
        valid = namaValid(m->nama);
        if (!valid) {
            printf("  [!] Nama tidak valid. Hanya huruf, spasi, titik, atau tanda hubung.\n");
        }
    } while (!valid);
}

static void inputSemester(Mahasiswa *m)
{
    long nilai;
    int valid;
    char prompt[64];
    sprintf(prompt, "Semester (%d-%d)           : ", SEMESTER_MIN, SEMESTER_MAX);

    do {
        valid = bacaInt(prompt, &nilai);
        if (!valid) {
            printf("  [!] Input harus berupa angka bulat.\n");
        } else if (nilai < SEMESTER_MIN || nilai > SEMESTER_MAX) {
            printf("  [!] Semester harus antara %d sampai %d.\n", SEMESTER_MIN, SEMESTER_MAX);
            valid = 0;
        }
    } while (!valid);

    m->semester = (int)nilai;
}

static void inputIPK(Mahasiswa *m)
{
    float nilai = 0;
    int valid;

    do {
        valid = bacaFloat("IPK (0.00 - 4.00)       : ", &nilai);
        if (!valid) {
            printf("  [!] Input harus berupa angka (contoh: 3.75).\n");
        } else if (nilai < IPK_MIN || nilai > IPK_MAX) {
            printf("  [!] IPK harus antara %.2f sampai %.2f.\n", IPK_MIN, IPK_MAX);
            valid = 0;
        }
    } while (!valid);

    m->ipk = nilai;
}

static void inputPenghasilan(Mahasiswa *m)
{
    long nilai;
    int valid;

    do {
        valid = bacaInt("Penghasilan ortu (Rp/bln): ", &nilai);
        if (!valid) {
            printf("  [!] Input harus berupa angka bulat tanpa titik/koma (contoh: 3000000).\n");
        } else if (nilai < 0) {
            printf("  [!] Penghasilan tidak boleh negatif.\n");
            valid = 0;
        }
    } while (!valid);

    m->penghasilanOrtu = nilai;
}

static void inputTanggungan(Mahasiswa *m)
{
    long nilai;
    int valid;

    do {
        valid = bacaInt("Jumlah tanggungan ortu  : ", &nilai);
        if (!valid) {
            printf("  [!] Input harus berupa angka bulat.\n");
        } else if (nilai < 0 || nilai > TANGGUNGAN_MAX) {
            printf("  [!] Jumlah tanggungan harus antara 0 sampai %d.\n", TANGGUNGAN_MAX);
            valid = 0;
        }
    } while (!valid);

    m->jumlahTanggungan = (int)nilai;
}

static void inputPrestasi(Mahasiswa *m)
{
    long nilai;
    int valid;

    do {
        valid = bacaInt("Jumlah prestasi         : ", &nilai);
        if (!valid) {
            printf("  [!] Input harus berupa angka bulat.\n");
        } else if (nilai < 0 || nilai > PRESTASI_MAX) {
            printf("  [!] Jumlah prestasi harus antara 0 sampai %d.\n", PRESTASI_MAX);
            valid = 0;
        }
    } while (!valid);

    m->jumlahPrestasi = (int)nilai;
}

/* Fungsi utama bagian Mahasiswa 1: dipanggil oleh main / Mahasiswa 2 */
void inputDataMahasiswa(Mahasiswa *m)
{
    printf("========================================\n");
    printf("   INPUT DATA PENDAFTAR BEASISWA\n");
    printf("========================================\n");

    inputNRP(m);
    inputNama(m);
    inputSemester(m);
    inputIPK(m);

    printf("\n--- Data Pendukung ---\n");
    inputPenghasilan(m);
    inputTanggungan(m);
    inputPrestasi(m);
}

/* Tampilkan ulang data yang sudah diinput (untuk konfirmasi) */
void tampilkanDataMahasiswa(const Mahasiswa *m)
{
    printf("\n========================================\n");
    printf("          DATA MAHASISWA\n");
    printf("========================================\n");
    printf("NRP                      : %s\n", m->nrp);
    printf("Nama                     : %s\n", m->nama);
    printf("Semester                 : %d\n", m->semester);
    printf("IPK                      : %.2f\n", m->ipk);
    printf("Penghasilan ortu (Rp/bln): %ld\n", m->penghasilanOrtu);
    printf("Jumlah tanggungan ortu   : %d\n", m->jumlahTanggungan);
    printf("Jumlah prestasi          : %d\n", m->jumlahPrestasi);
    printf("========================================\n");
}

/* ###################################################################### */
/* ##  MAHASISWA 2 : PROSES SELEKSI                                     ## */
/* ##  Kriteria penerima, cek IPK minimum & syarat lainnya              ## */
/* ##  Menentukan status memenuhi / tidak memenuhi (if-else, for)       ## */
/* ###################################################################### */

/* ===================== KRITERIA SELEKSI ===================== */
/* Ubah angka di sini kalau ketentuan berubah */
#define SELEKSI_IPK_MIN          3.00f      /* IPK minimum */
#define SELEKSI_SEMESTER_MIN     3          /* minimal semester 3 */
#define SELEKSI_SEMESTER_MAX     7          /* maksimal semester 7 */
#define SELEKSI_PENGHASILAN_MAX  5000000L   /* penghasilan ortu maks Rp/bulan */
#define SELEKSI_PRESTASI_MIN     1          /* minimal 1 prestasi */

#define JUMLAH_KRITERIA  5
#define MAX_DESKRIPSI    120

/* ===================== STRUCT HASIL ===================== */

/* Hasil pemeriksaan satu kriteria */
typedef struct {
    int  lolos;                         /* 1 = terpenuhi, 0 = tidak */
    char deskripsi[MAX_DESKRIPSI];      /* alasan / penjelasan */
} HasilKriteria;

/* Hasil seleksi satu mahasiswa */
typedef struct {
    int           memenuhi;                    /* 1 = MEMENUHI, 0 = TIDAK MEMENUHI */
    int           jumlahLolos;                 /* banyak kriteria terpenuhi */
    HasilKriteria kriteria[JUMLAH_KRITERIA];   /* detail tiap kriteria */
} HasilSeleksi;

/* ===================== PROSES SELEKSI ===================== */
/*
 * KRITERIA PENERIMA BEASISWA (semua harus terpenuhi):
 *   1. IPK         >= SELEKSI_IPK_MIN
 *   2. Semester    antara SELEKSI_SEMESTER_MIN s/d SELEKSI_SEMESTER_MAX
 *   3. Penghasilan ortu <= SELEKSI_PENGHASILAN_MAX per bulan
 *   4. Prestasi    >= SELEKSI_PRESTASI_MIN
 *   5. Data tanggungan ortu (informasi pendukung, selalu tercatat)
 */

/* Isi satu entri kriteria */
static void setKriteria(HasilSeleksi *h, int idx, int lolos, const char *teks)
{
    h->kriteria[idx].lolos = lolos;
    snprintf(h->kriteria[idx].deskripsi, MAX_DESKRIPSI, "%s", teks);
}

/* Proses seleksi satu mahasiswa -> mengisi struct HasilSeleksi */
void prosesSeleksi(const Mahasiswa *m, HasilSeleksi *h)
{
    char teks[MAX_DESKRIPSI];

    memset(h, 0, sizeof(*h));

    /* --- Kriteria 1: IPK minimum (toleransi kecil untuk float) --- */
    if (m->ipk + 0.0001f >= SELEKSI_IPK_MIN) {
        snprintf(teks, sizeof(teks), "IPK %.2f memenuhi minimum %.2f",
                 m->ipk, SELEKSI_IPK_MIN);
        setKriteria(h, 0, 1, teks);
    } else {
        snprintf(teks, sizeof(teks), "IPK %.2f di bawah minimum %.2f",
                 m->ipk, SELEKSI_IPK_MIN);
        setKriteria(h, 0, 0, teks);
    }

    /* --- Kriteria 2: rentang semester --- */
    if (m->semester < SELEKSI_SEMESTER_MIN) {
        snprintf(teks, sizeof(teks), "Semester %d terlalu awal (minimal semester %d)",
                 m->semester, SELEKSI_SEMESTER_MIN);
        setKriteria(h, 1, 0, teks);
    } else if (m->semester > SELEKSI_SEMESTER_MAX) {
        snprintf(teks, sizeof(teks), "Semester %d melewati batas (maksimal semester %d)",
                 m->semester, SELEKSI_SEMESTER_MAX);
        setKriteria(h, 1, 0, teks);
    } else {
        snprintf(teks, sizeof(teks), "Semester %d berada dalam rentang %d-%d",
                 m->semester, SELEKSI_SEMESTER_MIN, SELEKSI_SEMESTER_MAX);
        setKriteria(h, 1, 1, teks);
    }

    /* --- Kriteria 3: penghasilan orang tua --- */
    if (m->penghasilanOrtu <= SELEKSI_PENGHASILAN_MAX) {
        snprintf(teks, sizeof(teks), "Penghasilan ortu Rp%ld/bln (batas Rp%ld)",
                 m->penghasilanOrtu, SELEKSI_PENGHASILAN_MAX);
        setKriteria(h, 2, 1, teks);
    } else {
        snprintf(teks, sizeof(teks), "Penghasilan ortu Rp%ld/bln melebihi batas Rp%ld",
                 m->penghasilanOrtu, SELEKSI_PENGHASILAN_MAX);
        setKriteria(h, 2, 0, teks);
    }

    /* --- Kriteria 4: prestasi --- */
    if (m->jumlahPrestasi >= SELEKSI_PRESTASI_MIN) {
        snprintf(teks, sizeof(teks), "Memiliki %d prestasi (minimal %d)",
                 m->jumlahPrestasi, SELEKSI_PRESTASI_MIN);
        setKriteria(h, 3, 1, teks);
    } else {
        snprintf(teks, sizeof(teks), "Prestasi %d, kurang dari minimal %d",
                 m->jumlahPrestasi, SELEKSI_PRESTASI_MIN);
        setKriteria(h, 3, 0, teks);
    }

    /* --- Kriteria 5: data tanggungan (informasi pendukung) --- */
    snprintf(teks, sizeof(teks), "Jumlah tanggungan ortu %d tercatat",
             m->jumlahTanggungan);
    setKriteria(h, 4, 1, teks);

    /* --- Status akhir: perulangan semua kriteria --- */
    h->jumlahLolos = 0;
    for (int i = 0; i < JUMLAH_KRITERIA; i++) {
        if (h->kriteria[i].lolos) h->jumlahLolos++;
    }

    if (h->jumlahLolos == JUMLAH_KRITERIA) {
        h->memenuhi = 1;
    } else {
        h->memenuhi = 0;
    }
}

/* ###################################################################### */
/* ##  MAHASISWA 3 : HASIL & PENGUJIAN  (BELUM DIKERJAKAN)              ## */
/* ##  Tampilkan hasil + alasan, buat rekap, uji batas, integrasi       ## */
/* ##  Data hasil ada di struct HasilSeleksi (lihat bagian Mahasiswa 2) ## */
/* ###################################################################### */

/* TODO Mahasiswa 3: tulis fungsi tampilan hasil & rekap di sini,
   contoh: void tampilkanHasilSeleksi(const Mahasiswa *m, const HasilSeleksi *h);
   Isi yang bisa dipakai: h->memenuhi, h->jumlahLolos, h->kriteria[i].lolos,
   h->kriteria[i].deskripsi */

/* ###################################################################### */
/* ##  MAIN : PENGHUBUNG M1 -> M2 -> M3                                 ## */
/* ###################################################################### */
int main(void)
{
    Mahasiswa    mhs;
    HasilSeleksi hasil;
    char         ulang[MAX_BUFFER];

    do {
        memset(&mhs, 0, sizeof(mhs));

        /* ---- Mahasiswa 1 ---- */
        inputDataMahasiswa(&mhs);
        tampilkanDataMahasiswa(&mhs);

        /* ---- Mahasiswa 2 ---- */
        prosesSeleksi(&mhs, &hasil);

        /* ---- Mahasiswa 3 (TODO): panggil fungsi tampilan hasil di sini ---- */

        bacaBaris("\nInput mahasiswa lain? (y/n): ", ulang, sizeof(ulang));
        printf("\n");
    } while (ulang[0] == 'y' || ulang[0] == 'Y');

    printf("Selesai.\n");
    return 0;
}