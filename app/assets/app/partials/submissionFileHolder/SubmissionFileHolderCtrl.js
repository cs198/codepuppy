angular.module('codepuppy').controller('SubmissionFileHolderCtrl',
['$scope', '$stateParams', '$fileUploader', '$http', '$modal',
function($scope, $stateParams, $fileUploader, $http, $modal)
{
  $scope.comments = {};

  $scope.getComments = function() {
    if ($scope.file === undefined) return;

    var urlParams = {
      submission_file_id: $scope.file.id
    };

    $http({
      method: 'GET',
      url: '/submission_files/' + $scope.file.id + '/file_comments.json',
      data: urlParams
    }).success(function(data, status, headers, config) {
      $scope.comments = {};
      for (var i = 0; i < data.length; ++i) {
        var newComment = data[i];
        $scope.comments[newComment.line_number] = newComment;
      }
    });
  };

  $scope.lineLoaded = function(index, line) {
    if(index === $scope.fileLines.length - 1) {
      Prism.highlightAll();
    }
  };

  $scope.submissionClicked = function(index) {
    if ($scope.commentPermissions) {
      $scope.commentBody = {};
      if ($scope.comments[index] !== undefined) {
        return;
      } else {
        $scope.commentBody.line_number = index;
        $scope.commentBody.comment = '';
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
        var createComment = function() {
          var urlParams = {
            submission_file_id: $scope.file.id,
            line_number: $scope.commentBody.line_number,
            num_lines: 1,
            comment: $scope.commentBody.comment
          };

          $http({
            method: 'POST',
            url: '/submission_files/' + $scope.file.id + '/file_comments.json',
            data: urlParams
          }).success(function(data, status, headers, config) {
            // data returned should have commentBody's line number and content.
            // Set commentBody equal to data so it becomes associated with id.
            $scope.commentBody = data;
            $scope.comments[$scope.commentBody.line_number] =
                $scope.commentBody;
          });
        };
        if (comment !== '') {
          createComment();
        }
      });
    }
  };
}]);
