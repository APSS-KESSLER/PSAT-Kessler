import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.stream.Collectors;
import java.io.*;

public class ProcessHtml {
	public static void main(String... args) throws IOException {
		System.out.println(Files.lines(Path.of("./page.html"), StandardCharsets.UTF_8)
			.filter(s -> !s.isBlank())
			.map(s -> s.replace("\t", "").replace("\"", "\\\""))
			.collect(Collectors.joining()));
	}
}