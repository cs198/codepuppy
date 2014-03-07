angular.module('codepuppy').controller('FileUploaderCtrl',
['$scope', '$routeParams', '$fileUploader', function($scope, $routeParams, $fileUploader) {
  
  console.log("reached");
  var uploader = $scope.uploader = $fileUploader.create({
      scope: $scope,                         
  });
  
  uploader.bind('afteraddingfile', function (event, item) {
      console.info('After adding a file', item);
  });

  uploader.bind('afteraddingall', function (event, items) {
      console.info('After adding all files', items);
  });

  uploader.bind('beforeupload', function (event, item) {
      console.info('Before upload', item);
  });

  uploader.bind('progress', function (event, item, progress) {
      console.info('Progress: ' + progress, item);
  });

  uploader.bind('success', function (event, xhr, item, response) {
      console.info('Success', xhr, item, response);
  });

  uploader.bind('cancel', function (event, xhr, item) {
      console.info('Cancel', xhr, item);
  });

  uploader.bind('error', function (event, xhr, item, response) {
      console.info('Error', xhr, item, response);
  });

  uploader.bind('complete', function (event, xhr, item, response) {
      console.info('Complete', xhr, item, response);
  });

  uploader.bind('progressall', function (event, progress) {
      console.info('Total progress: ' + progress);
  });

  uploader.bind('completeall', function (event, items) {
      console.info('Complete all', items);
  });

  
}]);
