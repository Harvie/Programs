var reload = function(responseHandler) {
    window.location.href = window.location.href;
    responseHandler(null);
};
var execute = function(code, responseHandler) {
    var exception = null;
    try {
        eval(code);
    } catch (exc) {
      // there was a circular reference in jQueries exceptions that
      // sendResponse didn't like
      exception = {name: exc.name, message: exc.message};
    }
    responseHandler(exception);
};

chrome.extension.onRequest.addListener(
        function(data, sender, responseHandler) {
    if (data.reload) {
        reload(responseHandler);
    } else if (data.code) {
        execute(data.code, responseHandler);
    } else {
        responseHandler({name: "InvalidArgumentException",
                message: "Argument to content script not understood."});
    }
});
