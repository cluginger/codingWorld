import kotlin.system.exitProcess

plugins {
    java
    antlr
    idea
}

repositories {
    mavenCentral()
    maven { setUrl("https://clojars.org/repo") }
}

dependencies {
    testImplementation("com.google.code.gson:gson:2.8.6")
    testImplementation(platform("org.junit:junit-bom:5.7.1"))
    testImplementation("org.junit.jupiter:junit-jupiter")
    antlr("org.antlr:antlr4:4.9.1")
}

tasks {
    "test"(Test::class) {
        useJUnitPlatform()
    }
}

tasks.register("runProgram", JavaExec::class) {
    main = "at.tugraz.ist.soma.Main"

    var fileName = ""
    var criterion = ""
    var debug = false

    if (project.hasProperty("fileName")) {
        fileName = project.property("fileName").toString()
    } else {
        throw GradleException("Error: Enter a fileName property")

    }
    if (project.hasProperty("criterion")) {
        criterion = project.property("criterion").toString()
    } else {
        throw GradleException("Error: Enter a criterion property")
    }

    if (project.hasProperty("debug")) {
        debug = true;
    } else{
        println("wtf")
    }
    if (!debug) {
        args = listOf(criterion, fileName )
    } else {
        args = listOf( criterion, fileName, "debug")
    }

    classpath = sourceSets["main"].runtimeClasspath
}

tasks.generateGrammarSource {
    arguments = arguments + listOf("-visitor", "-long-messages")
}

idea.module {
    generatedSourceDirs.add(file("build/generated-src/antlr/main"))
}



java {
    sourceCompatibility = JavaVersion.VERSION_11
    targetCompatibility = JavaVersion.VERSION_11
}
