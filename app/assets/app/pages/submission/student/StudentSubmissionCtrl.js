angular.module('codepuppy').controller('StudentSubmissionCtrl', ['$scope', '$stateParams', '$modal', '$http',
                                       function($scope, $stateParams, $modal, $http) {
  $scope.message = 'I\'m a submission page for the course ' +
    $stateParams.courseID;
  $scope.message += ' and assignment ' + $stateParams.assignmentID;
  $scope.message += ' and submission ' + $stateParams.submissionID;

  // Map from line numbers to commentBody objects -- have .comment and .lineNumber fields
  $scope.comments = {};
  $scope.commentsReleased = false;
  $scope.commentPermissions = false;
  $scope.files = [];

  var getSubmission = function() {
    $http({method: 'GET', url: '/submissions/' + $stateParams.submissionID + '.json'})
    .success(function(data, status, headers, config) {
      $scope.commentsReleased = data.submission.feedback_released;
    });
  };

  var getFiles = function() {
    $http({method: 'GET', url: '/submissions/' + $stateParams.submissionID + '/submission_files.json'})
    .success(function(data, status, headers, config) {
      for(var i = 0; i < data.length; ++i) {
        $scope.files.push(data[i]);
      }
      if($scope.files.length > 0) {
        $scope.selectedFile = $scope.files[0];
      }

    });
  };

  getFiles();
  getSubmission();

}]);
