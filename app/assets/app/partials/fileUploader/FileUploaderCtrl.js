angular.module('codepuppy').controller('FileUploaderCtrl',
['$scope', '$routeParams', '$fileUploader', '$http', function($scope, $routeParams, $fileUploader, $http) {

  var uploader = $scope.uploader = $fileUploader.create({
      scope: $scope,
      url: '/submission_files.json',             
  });
  

  uploader.bind('completeall', function (event, items) {
      // TODO: redirect
      console.info('Complete all', items);

      // Redirect
  });

  $scope.submitAll = function()
  {
    var createSubmission = function() {
      //TODO: Fix ID to be student ID when logged in.
      var date = new Date();
      var urlParams = {
        student_id: 1,
        assignment_id: $routeParams.assignmentID,
        feedback_released: false,
        date_submitted: date.toJSON()
      };


      // 4 URL params: assignment_id, student_id, date_submited, feedback_released
      $http({method: 'POST', url: '/submissions.json', data: urlParams}).success(function(data, status, headers, config) {
        console.log(data);
        // Add submission ID to the file data
        $scope.uploader.formData.push({ submission_id: data.id });
        
        // Submit each member of the queue 
        $scope.uploader.uploadAll();
      });
      
    };

    createSubmission();
  };

}]);
