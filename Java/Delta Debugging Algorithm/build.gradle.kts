plugins {
    java
    idea
}

repositories {
    mavenCentral()
    maven { setUrl("https://clojars.org/repo") }
}

tasks.withType<Test>().configureEach {
    useJUnitPlatform()
}


dependencies {
    testImplementation("com.google.code.gson:gson:2.8.6")

    testImplementation("org.junit.jupiter:junit-jupiter-api:5.4.2")
    testImplementation("org.junit.jupiter:junit-jupiter-params:5.4.2")

    testRuntimeOnly("org.junit.jupiter:junit-jupiter-engine:5.4.2")
}

tasks.register("runProgram", JavaExec::class) {
    main = "at.tugraz.ist.soma.dd.Main"

    var fileName = ""
    var algo = ""
    var input = ""
    var fail = ""
    var deps = ""

    if (project.hasProperty("fileName")) {
        fileName = project.property("fileName").toString()
        args = listOf(fileName)
    } else {
        if (project.hasProperty("algo")) {
            algo = project.property("algo").toString()
        } else {
            throw GradleException("Error: Enter the algo property")
        }
        if (project.hasProperty("input")) {
            input = project.property("input").toString()
        } else {
            throw GradleException("Error: Enter the input property")
        }

        if (project.hasProperty("fail")) {
            fail = project.property("fail").toString()
        } else {
            throw GradleException("Error: Enter the fail property")
        }
        if (project.hasProperty("deps")) {
            deps = project.property("deps").toString()
        } else {
            throw GradleException("Error: Enter the deps property")
        }

        args = listOf(algo, input, fail, deps)
    }

    classpath = sourceSets["main"].runtimeClasspath
}

java {
    sourceCompatibility = JavaVersion.VERSION_11
    targetCompatibility = JavaVersion.VERSION_11
}
