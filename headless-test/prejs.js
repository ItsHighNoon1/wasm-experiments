var Module = {
    // Docs suggest to me that this has to be true otherwise
    // the C runtime will fall apart when main terminates
    // but my testing shows no difference. stdout is cleaned
    // up either way, the callback is not cleaned up either
    // way, I guess I'll just keep it true.
    noExitRuntime: true,
}