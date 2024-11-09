package virtualization

import kotlinx.serialization.Serializable
import kotlinx.serialization.decodeFromString
import kotlinx.serialization.json.Json

@Serializable
data class GetProgram(
    val code: List<ULong>,
    val buf: Map<String, MutableList<Int>>
    // Add more fields based on your JSON structure
)

fun loadJsonData(): GetProgram {
    val lines = object {}.javaClass.getResourceAsStream("/program.json")
    val reader = lines?.bufferedReader()
    val text = reader?.readText()
    return Json.decodeFromString<GetProgram>(text!!)
}