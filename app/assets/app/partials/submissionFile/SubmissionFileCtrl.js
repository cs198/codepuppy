angular.module('codepuppy').controller('SubmissionFileCtrl',
['$scope', '$routeParams', '$fileUploader', '$http', '$modal', function($scope, $routeParams, $fileUploader, $http, $modal) {
  console.log($scope.file);
  $scope.comments = {};
  
  if($scope.commentPermissions || $scope.commentReleased) {
    // Make get request for comments.
  }

  $scope.submissionClicked = function(index) {
    if($scope.commentPermissions) {
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
        // Make a POST for a new comment.
        // On succcess: perform these things -- add to comments.

        $scope.commentBody.comment = comment;
        $scope.comments[$scope.commentBody.lineNumber] = $scope.commentBody;
      });
    }
  };
}]);
