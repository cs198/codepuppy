angular.module('codepuppy').controller('FileUploaderCtrl',
['$scope', '$routeParams', '$fileUploader', '$http', function($scope, $routeParams, $fileUploader, $http) {

  
  
  var uploader = $scope.uploader = $fileUploader.create({
      scope: $scope,
      url: '/submission_files.json',             
  });
  
  uploader.bind('afteraddingfile', function (event, item) {
      //console.info('After adding a file', item);
  });

  uploader.bind('afteraddingall', function (event, items) {
      //console.info('After adding all files', items);
  });

  uploader.bind('beforeupload', function (event, item) {
      //console.info('Before upload', item);
  });

  uploader.bind('progress', function (event, item, progress) {
      //console.info('Progress: ' + progress, item);
  });

  uploader.bind('success', function (event, xhr, item, response) {
      //console.info('Success', xhr, item, response);
  });

  uploader.bind('cancel', function (event, xhr, item) {
      //console.info('Cancel', xhr, item);
  });

  uploader.bind('error', function (event, xhr, item, response) {
      //console.info('Error', xhr, item, response);
      // TODO: handle error
  });

  uploader.bind('complete', function (event, xhr, item, response) {
      //console.info('Complete', xhr, item, response);
  });

  uploader.bind('progressall', function (event, progress) {
      //console.info('Total progress: ' + progress);
  });

  uploader.bind('completeall', function (event, items) {
      // TODO: redirect
      //console.info('Complete all', items);

      // Redirect
  });

  $scope.submitAll = function()
  {
    var createSubmission = function() {
      // 4 URL params: assignment_id, student_id, date_submited, feedback_released
      $http({method: 'POST', url: '/submissions.json'}).success(function(data, status, headers, config) {
        console.log(data);
        // Add submission ID to the file data
        // $scope.uploader.formData.push(data.id);
        
        // Submit each member of the queue 
        // $scope.uploader.uploadAll();
      });
      
    };

    createSubmission();
  };

}]);
