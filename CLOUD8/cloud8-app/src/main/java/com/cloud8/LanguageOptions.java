package com.cloud8;

import java.util.Locale;

public class LanguageOptions {
    Locale english;
    Locale swedish;
    Locale spanish;
    Locale urdu;
    Locale farsi;
    Locale[] langOpts;

    public LanguageOptions() {
        this.english = new Locale("en_GB");
        this.swedish = new Locale("se_SE");
        this.spanish = new Locale("es_ES");
        this.urdu = new Locale("pk_UR");
        this.farsi = new Locale("ir_FA");
        this.langOpts = new Locale[] {this.english, this.swedish, this.spanish, this.urdu, this.farsi};
    }

    public Locale[] getLangOpts() {
        return this.langOpts;
    }
}
