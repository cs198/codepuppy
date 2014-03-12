angular.module('codepuppy').controller('SubmissionFileCtrl',
['$scope', '$routeParams', '$fileUploader', '$http', '$location', function($scope, $routeParams, $fileUploader, $http, $location) {

  $scope.comments = {};
  console.log("reached");

  if($scope.commentPermissions) {
    $scope.submissionClicked = function(index) {

      $scope.commentBody = {};
      if($scope.comments[index] !== undefined) {
        $scope.commentBody = $scope.comments[index];
      } else {
        $scope.commentBody.lineNumber = index;
        $scope.commentBody.comment = "Hello!";
      }

      var commentModal = $modal.open({
      templateUrl: '/assets/partials/commentModal/commentModal.html',
      controller: CommentModalCtrl,
      resolve: {
        commentBody: function()  {
          return $scope.commentBody;
        }
      }
      });

      commentModal.result.then(function(comment) {
        $scope.commentBody.comment = comment;
        // Used to be push
        console.log($scope.commentBody);
        $scope.comments[$scope.commentBody.lineNumber] = $scope.commentBody;
      });
    };
  }

  if($scope.commentPermissions || $scope.commentReleased) {
    // Make get request for comments.
  }

  // TODO: Change to an API call
  //$scope.codeLines = [];
  //$.get('/assets/pages/submission/code.java', function(data) {
  //$scope.codeLines = $scope.file.data.split('\n');

  //});

}]);
